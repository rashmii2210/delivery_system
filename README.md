# 🚚 Delivery Assignment System

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![FastAPI](https://img.shields.io/badge/FastAPI-Backend-green)
![Algorithm](https://img.shields.io/badge/Algorithm-Dijkstra-orange)
![Status](https://img.shields.io/badge/Status-Completed-success)

A hybrid **FastAPI + C++ delivery optimization platform** that assigns delivery orders to riders using graph-based shortest-path routing. The API layer handles request validation and orchestration, while a native C++ engine performs route computation and rider assignment for improved performance.

---

# 📑 Table of Contents

* [Overview](#overview)
* [Demo](#demo)
* [Architecture](#architecture)
* [Features](#features)
* [Technology Stack](#technology-stack)
* [Algorithms](#algorithms)
* [Project Structure](#project-structure)
* [Getting Started](#getting-started)
* [API Endpoints](#api-endpoints)
* [Benchmark Results](#benchmark-results)
* [Example Workflow](#example-workflow)
* [Engineering Decisions](#engineering-decisions)
* [Known Limitations](#known-limitations)
* [Future Improvements](#future-improvements)
* [Key Learnings](#key-learnings)
* [Highlights](#highlights)

---

# Overview

Modern delivery platforms must efficiently assign orders while minimizing travel distance and balancing rider workload.

This project models a city as a weighted graph and combines:

* **Dijkstra's Algorithm** for shortest-path computation
* **Priority-aware rider assignment**
* **FastAPI REST endpoints**
* **Native C++ optimization engine**
* **Benchmarking support for large-scale simulations**

The architecture separates API concerns from computational logic, allowing the routing engine to evolve independently from the web layer.

---

## Demo
<p align="center">
  <video src="https://github.com/user-attachments/assets/a4958697-a7c4-4908-8678-09b7fc54208b" width="500" autoplay loop controls playsinline></video>
</p>

The demo showcases:

* Rider assignment using `/assign`
* Shortest-path routing with Dijkstra's Algorithm
* Workload balancing across riders
* Performance benchmarking using `/benchmark`

---

# Architecture

```text
                    ┌─────────────┐
                    │   Client    │
                    └──────┬──────┘
                           │ HTTP
                           ▼
                  ┌─────────────────┐
                  │ FastAPI Backend │
                  │   (Python)      │
                  └──────┬──────────┘
                         │
                  Writes input.json
                         │
                         ▼
                ┌─────────────────┐
                │  delivery.exe   │
                │     C++17       │
                └──────┬──────────┘
                       │
                Computes:
                • Graph Routing
                • Dijkstra
                • Order Assignment
                       │
                 Writes output.json
                       │
                       ▼
                 FastAPI Response
```

---

# Features

## Graph-Based Route Optimization

* Models delivery networks as weighted graphs
* Supports arbitrary city layouts
* Computes shortest delivery routes using Dijkstra's Algorithm

## Rider Assignment Engine

* Assigns orders dynamically to available riders
* Considers:

  * Travel distance
  * Rider workload
  * Order priority

## Benchmark Mode

* Generates random delivery networks
* Simulates large-scale workloads
* Measures assignment performance

## API-Driven Design

* RESTful endpoints
* Request validation using Pydantic
* Interactive Swagger documentation

---

# Technology Stack

| Layer              | Technology                          |
| ------------------ | ----------------------------------- |
| Backend API        | FastAPI                             |
| Validation         | Pydantic                            |
| Routing Engine     | C++17                               |
| JSON Serialization | nlohmann/json                       |
| Algorithms         | Dijkstra, Priority-Based Assignment |
| Communication      | Subprocess + JSON IPC               |

---

# Algorithms

## Shortest Path Routing

The routing engine uses Dijkstra's Algorithm to compute minimum-distance paths between locations.

Complexity:

```text
O((V + E) log V)
```

Where:

* V = Number of Nodes
* E = Number of Edges

## Rider Assignment

Orders are assigned using a priority-aware scoring strategy that balances:

```text
Assignment Score =
Distance Cost +
Current Rider Workload +
Order Priority Weight
```

The rider with the best score receives the order.

---

# Project Structure

```text
delivery_system/
└── backend/
    ├── main.py
    ├── requirements.txt
    ├── Dockerfile
    └── cpp_engine/
        ├── main.cpp
        ├── graph.h
        ├── graph.cpp
        ├── dijkstra.h
        ├── dijkstra.cpp
        ├── assignment.h
        ├── assignment.cpp
        ├── generator.h
        ├── generator.cpp
        ├── io_handler.h
        ├── io_handler.cpp
        ├── rider.h
        ├── order.h
        ├── json.hpp
        └── delivery.exe
```

---

# Getting Started

## Prerequisites

* Python 3.10+
* Precompiled `delivery.exe`

## Installation

```bash
cd backend

python -m venv venv

# Windows
venv\Scripts\activate

pip install -r requirements.txt

uvicorn main:app --reload
```

API:

```text
http://127.0.0.1:8000
```

Swagger Docs:

```text
http://127.0.0.1:8000/docs
```

---

# API Endpoints

## POST /assign

Assign delivery orders to riders.

### Input

```json
{
  "nodes": [...],
  "edges": [...],
  "riders": [...],
  "orders": [...]
}
```

### Example Response

```json
{
  "riders": [
    {
      "id": 0,
      "name": "Rider1",
      "workload": 2,
      "currentLocation": "B"
    }
  ],
  "totalOrders": 2,
  "totalRiders": 1,
  "timeTakenMs": 0
}
```

---

## POST /benchmark

Generate synthetic delivery networks and evaluate assignment performance.

### Request

```json
{
  "numNodes": 100,
  "numEdges": 500,
  "numRiders": 20,
  "numOrders": 1000
}
```

### Returns

* Execution time
* Rider load distribution
* Assignment statistics

---

# Benchmark Results

Example benchmark configuration:

```json
{
  "numNodes": 100,
  "numEdges": 500,
  "numRiders": 20,
  "numOrders": 1000
}
```

Example output from this project:

```json
{
  "timeTakenMs": 20,
  "totalOrders": 1000,
  "totalRiders": 20
}
```

The routing engine successfully processed **1000 delivery orders across a 100-node graph in approximately 20 milliseconds**, demonstrating efficient shortest-path computation and rider assignment performance.

---

# Example Workflow

## Input

```text
Hub ----5---- A
 |
 8
 |
 B
```

Orders:

```text
Order 0: Hub → A
Order 1: Hub → B
```

Rider:

```text
Rider1 starts at Hub
```

## Output

```text
Order 0 → Rider1
Order 1 → Rider1

Final Location: B
Workload: 2
```

---

# Engineering Decisions

## Why C++?

Shortest-path routing is computationally intensive and benefits from native execution speed.

Using C++ for graph traversal allows the system to scale more effectively than a pure Python implementation.

## Why FastAPI?

FastAPI provides:

* Automatic validation
* OpenAPI documentation
* High-performance APIs
* Simple integration with external services

## Why JSON-Based IPC?

The routing engine remains independently executable and testable from the command line.

Benefits:

* Language agnostic
* Easy debugging
* Clear separation of concerns

---

# Known Limitations

Current implementation requires:

* Node IDs to be contiguous
* Rider IDs to be contiguous
* Order IDs to be contiguous

Supported:

```text
0,1,2,3
```

Not Supported:

```text
1,5,10
```

A future enhancement would introduce ID remapping inside the I/O layer.

---

# Future Improvements

* gRPC-based communication between Python and C++
* Real-time rider tracking
* Multi-order route batching
* Min-Cost Max-Flow optimization
* Vehicle Routing Problem (VRP) solver
* Dockerized deployment pipeline
* CI/CD using GitHub Actions

---

# Key Learnings

* Hybrid Python + C++ system architecture
* Graph algorithms in production-style workflows
* FastAPI backend development
* Cross-language communication using JSON IPC
* Performance benchmarking and scalability testing
* Route optimization fundamentals
* API design and request validation

---

# Highlights

This project demonstrates:

* Graph Algorithms (Dijkstra's Algorithm)
* C++ Systems Programming
* FastAPI Backend Development
* Cross-Language Architecture (Python ↔ C++)
* Performance Benchmarking
* Delivery Route Optimization
* REST API Design
