# Øving 7 - ALT og Djikstras

## Getting started
Prosjektet antar at du har en mappe med følgende tekstfiler:
- interessepkt.txt
- kanter.txt
- noder.txt
- landmarks.txt (landemerkene som ALT bruker, denne har jeg lagt ved på blackboard)

### Bygge prosjektet
```sh
mkdir build
cd build
CMake ..
make
```

### Kjøre applikasjonen
```sh
./djikstras <path_to_folder> <from_landmark> <to_landmark>
./alt <path_to_folder> <from_landmark> <to_landmark>
./closest_landmarks <path_to_folder> <landmark/node_num> <num_landmarks>
```

#### Eksempel:
##### Island
```sh
./dijkstra ../data/island "Alda Hotel Reykjavik" "Höfn"
./alt ../data/island "Alda Hotel Reykjavik" "Höfn"
```

##### Norden
```sh
./dijkstra ../data/norden "Trondheim" "Oslo"
./alt ../data/norden "Trondheim" "Oslo"

# Gløshaugen–Otilienborg
./dijkstra ../data/norden "Gløshaugen" "Otilienborg"
./alt ../data/norden "Gløshaugen" "Otilienborg"

# Fosnavåg–Espoo
./dijkstra ../data/norden "Fosnavåg" "Espoo"
./alt ../data/norden "Fosnavåg" "Espoo"
```

##### Landemerker
```sh
# 4 ladestasjoner nærmest Levanger (node 2106148):
./closest_landmarks ../data/norden Ladestasjon 2106148 4

# 4 drikkestedene nærmest gløshaugen (node 2001238)
./closest_landmarks ../data/norden Drikkested 2001238 4

# 4 spisesteder nærmest "Åre Björnen"
./closest_landmarks ../data/norden Spisested "Åre Björnen" 4

# Island
./closest_landmarks ../data/island Spisested "Alda Hotel Reykjavik" 4
```

### Output
Etter programmet har kjørt skrives ut tiden algoritmen brukte, antall noder besøkt, og avstand/tid mellom start- og målnoden.
