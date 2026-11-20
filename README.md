# Smart Emergency Ambulance Routing System

A simple college DAA project with a browser dashboard and a standalone C implementation.

## Features
- City road network represented as a weighted graph.
- Dijkstra's algorithm finds shortest routes.
- Greedy selection chooses the nearest available ambulance.
- Greedy selection chooses the nearest hospital with available beds.
- Insertion Sort ranks candidate routes by distance.
- Dashboard, dispatch page, ambulance and hospital lists, and a Dijkstra visualizer.

## Run the website

Run these commands from the repository root:

```bash
python3 -m http.server 8787
```

Open **http://localhost:8787** in your browser.

You can also run:

```bash
chmod +x run.sh
./run.sh
```

## Compile and run the C DAA program

Run from the repository root:

```bash
make
./ambulance_engine
```

Or compile manually:

```bash
cc -std=c11 -Wall -Wextra -pedantic -O2 main.c engine.c -o ambulance_engine
./ambulance_engine
```

Clean the generated executable with:

```bash
make clean
```

## Algorithms used

1. **Dijkstra:** Finds minimum distances in the weighted road graph. The simple matrix implementation takes O(V²) time.
2. **Greedy selection:** Selects the currently closest valid ambulance and hospital using the computed distances.
3. **Insertion Sort:** Ranks candidate indexes by route cost; worst-case time is O(n²).
4. **Constraint checks:** Skips unavailable ambulances and hospitals without beds.

## Project structure

- `index.html` - dashboard pages and layout
- `style.css` - dashboard styling
- `app.js` - browser-side routing and interactions
- `main.c` - sample graph and C program entry point
- `engine.c` / `engine.h` - DAA algorithms and data structures
- `Makefile` - C build commands
- `run.sh` - starts the local website

The website is a local simulation. It does not connect to real ambulance GPS, hospital systems, or emergency services.
