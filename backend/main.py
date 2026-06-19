from fastapi import FastAPI, HTTPException
from pydantic import BaseModel, Field
from typing import List
import subprocess
import json
import os

app = FastAPI(title="Delivery Assignment API")

CPP_ENGINE_DIR = r"D:\Projects\delivery_system\backend\cpp_engine"
EXE_PATH = os.path.join(CPP_ENGINE_DIR, "delivery.exe")
INPUT_PATH = os.path.join(CPP_ENGINE_DIR, "input.json")
OUTPUT_PATH = os.path.join(CPP_ENGINE_DIR, "output.json")


class Node(BaseModel):
    id: int
    name: str
    type: str

class Edge(BaseModel):
    from_: int = Field(..., alias="from")
    to: int
    dist: float

    class Config:
        populate_by_name = True

class RiderInput(BaseModel):
    id: int
    name: str
    startNode: int

class OrderInput(BaseModel):
    id: int
    pickup: int
    dropoff: int
    priority: int

class AssignmentRequest(BaseModel):
    nodes: List[Node]
    edges: List[Edge]
    riders: List[RiderInput]
    orders: List[OrderInput]

class BenchmarkRequest(BaseModel):
    numNodes: int = 100
    numEdges: int = 500
    numRiders: int = 50
    numOrders: int = 1000


@app.post("/assign")
def assign_orders(request: AssignmentRequest):
    input_data = {
        "nodes": [n.dict() for n in request.nodes],
        "edges": [{"from": e.from_, "to": e.to, "dist": e.dist} for e in request.edges],
        "riders": [r.dict() for r in request.riders],
        "orders": [o.dict() for o in request.orders],
    }

    with open(INPUT_PATH, "w") as f:
        json.dump(input_data, f, indent=2)

    try:
        result = subprocess.run(
            [EXE_PATH, INPUT_PATH, OUTPUT_PATH],
            capture_output=True,
            text=True,
            timeout=30
        )
    except subprocess.TimeoutExpired:
        raise HTTPException(status_code=500, detail="C++ engine timed out")

    if result.returncode != 0:
        raise HTTPException(status_code=500, detail=f"C++ engine failed: {result.stderr}")

    if not os.path.exists(OUTPUT_PATH):
        raise HTTPException(status_code=500, detail="Output file not generated")

    with open(OUTPUT_PATH, "r") as f:
        output_data = json.load(f)

    return output_data


@app.post("/benchmark")
def run_benchmark(request: BenchmarkRequest):
    """
    Runs a large-scale randomized test directly through the C++ engine.
    Useful for measuring system performance under load.
    """

    # Safety limits — prevent someone from crashing the server
    # with an absurdly large request
    if request.numNodes > 5000 or request.numOrders > 50000:
        raise HTTPException(
            status_code=400,
            detail="Benchmark size too large. Max: 5000 nodes, 50000 orders."
        )

    try:
        result = subprocess.run(
            [
                EXE_PATH,
                "benchmark",
                str(request.numNodes),
                str(request.numEdges),
                str(request.numRiders),
                str(request.numOrders),
                OUTPUT_PATH
            ],
            capture_output=True,
            text=True,
            timeout=60
        )
    except subprocess.TimeoutExpired:
        raise HTTPException(status_code=500, detail="Benchmark timed out (60s limit)")

    if result.returncode != 0:
        raise HTTPException(status_code=500, detail=f"Benchmark failed: {result.stderr}")

    if not os.path.exists(OUTPUT_PATH):
        raise HTTPException(status_code=500, detail="Benchmark output not generated")

    with open(OUTPUT_PATH, "r") as f:
        output_data = json.load(f)

    # Build a clean summary response (don't dump all 1000 orders by default)
    summary = {
        "config": {
            "numNodes": request.numNodes,
            "numEdges": request.numEdges,
            "numRiders": request.numRiders,
            "numOrders": request.numOrders,
        },
        "timeTakenMs": output_data.get("timeTakenMs"),
        "totalOrders": output_data.get("totalOrders"),
        "totalRiders": output_data.get("totalRiders"),
        "loadDistribution": [
            {"rider": r["name"], "orders": r["workload"]}
            for r in output_data.get("riders", [])
        ]
    }

    return summary


@app.get("/")
def root():
    return {
        "message": "Delivery Assignment API is running",
        "endpoints": {
            "POST /assign": "Submit custom graph/riders/orders for assignment",
            "POST /benchmark": "Run a large-scale randomized performance test"
        }
    }