void writeCoordintePathFromLandmarksToFile(Map &map, const string &startName, const string &endName, const string &outputFileName) {
    ofstream file(outputFileName);
    if (!file) {
        cout << "Error opening file: " << outputFileName << endl;
        return;
    }

    int start = map.nameTointerestPoint[startName].first;
    int end = map.nameTointerestPoint[endName].first;

    auto result = djikstras(map, start, end);

    cout << "Time from " << startName << " to " << endName << " is " << static_cast<double>(result.first) / 100 << " seconds." << endl;

    // Start of the GeoJSON file
    file << "{\n\"type\": \"FeatureCollection\",\n\"features\": [\n";
    file << "  {\n    \"type\": \"Feature\",\n";
    file << "    \"geometry\": {\n";
    file << "      \"type\": \"LineString\",\n";
    file << "      \"coordinates\": [";

    for (size_t i = 0; i < result.second.size(); ++i) {
        int node = result.second[i];
        // Write each coordinate as a JSON array
        file << "\n        [" << map.nodes[node].second << ", " << map.nodes[node].first << "]";
        if (i < result.second.size() - 1)
            file << ",";
    }

    // End of the GeoJSON file
    file << "\n      ]\n    },\n";
    file << "    \"properties\": {\n";
    file << "      \"name\": \"" << startName << " to " << endName << "\"\n";
    file << "    }\n  }\n]\n}";

    file.close();

    cout << "Done writing to file." << endl;
}