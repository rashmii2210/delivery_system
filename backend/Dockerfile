FROM python:3.11-slim

RUN apt-get update && apt-get install -y g++ && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

COPY . .

RUN g++ cpp_engine/main.cpp cpp_engine/graph.cpp cpp_engine/dijkstra.cpp \
    cpp_engine/assignment.cpp cpp_engine/io_handler.cpp \
    cpp_engine/generator.cpp cpp_engine/hungarian.cpp \
    cpp_engine/comparison.cpp -O2 -o cpp_engine/delivery

ENV PORT=8000
EXPOSE 8000

CMD ["sh", "-c", "uvicorn main:app --host 0.0.0.0 --port $PORT"]