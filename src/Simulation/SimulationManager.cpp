#include "SimulationManager.h"
#include <iostream>
#include <algorithm>

// namespace pour facilité l'écriture
namespace fs = std::filesystem;

// Récupérer l'arborescence des scènes
fs::path get_world_path() {
    // 1. On récupère le chemin vers l'exécutable (ou le dossier actuel)
    fs::path base_path = fs::current_path();

    // 2. Si on est dans "build", le dossier world est un niveau au-dessus
    if (base_path.filename() == "build") {
        return base_path.parent_path() / "world";
    }

    // 3. Si on est déjà à la racine, on entre simplement dans world
    if (fs::exists(base_path / "world")) {
        return base_path / "world";
    }

    // Par défaut, on tente un chemin relatif simple
    return fs::path("world");
}

// Path de travail par défaut
fs::path path = get_world_path();

// Dernière Modification du dossier
fs::file_time_type last_modif = fs::last_write_time(path); 

char Lexer::peek() {
    if (pos > source.length()) return '\0';
    return source[pos];
}

char Lexer::advance() {
    return source[pos++];
}

void Lexer::skipWhitespace() {
    while (pos < source.length()) {
        char c = source[pos];
        
        // Espaces classiques
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r') {
            pos++;
        }
        // Commentaire 
        else if (c == '/' && pos + 1 < source.length() && source[pos + 1] == '/') {
            pos += 2; // sauter le //
            while (pos < source.length() && source[pos] != '\n') {
                pos++;
            }
        }
        else {
            break;
        }
    }
}

Token Lexer::next() {
    skipWhitespace();
    
    if (pos >= source.length()) {
        return {TokenType::EndOfFile, ""};
    }

    char current = peek();

    if (current == ':') { advance(); return {TokenType::Colon, ":"}; }
    if (current == ',') { advance(); return {TokenType::Comma, ","}; }
    if (current == '(') { advance(); return {TokenType::LBrace, "("}; }
    if (current == ')') { advance(); return {TokenType::RBrace, ")"}; }
    if (current == '{') { advance(); return {TokenType::OBrace, "{"}; }
    if (current == '}') { advance(); return {TokenType::CBrace, "}"}; }

    if (std::isalpha(current)) {
        std::string id;
        while (pos < source.length() && (std::isalnum(peek()) || peek() == '_')) {
            id += advance();
        }
        return {TokenType::Identifier, id};
    }

    if (std::isdigit(current) || current == '-' || current == '.') {
        std::string num;
        if (current == '-') {
            num += advance();
        }
        while (pos < source.length() && (std::isdigit(peek()) || peek() == '.' || peek() == 'f' || peek() == 'k')) {
            num += advance();
        }
        return {TokenType::Number, num};
    }

    advance();
    return {TokenType::EndOfFile, ""};
}

float parseNumber(const std::string& numStr) {
    std::string cleaned = numStr;
    
    if (cleaned.back() == 'k' || cleaned.back() == 'f') {
        cleaned.pop_back();
    }
    
    return std::stof(cleaned);
}

Vector3 parseVector3(Lexer& lexer) {
    float x = 0, y = 0, z = 0;
    
    Token current = lexer.next();
    if (current.type == TokenType::LBrace) {
        current = lexer.next();
        if (current.type == TokenType::Number) {
            x = parseNumber(current.text);
        }
        
        current = lexer.next();
        if (current.type == TokenType::Comma) {
            current = lexer.next();
            if (current.type == TokenType::Number) {
                y = parseNumber(current.text);
            }
        }
        
        current = lexer.next();
        if (current.type == TokenType::Comma) {
            current = lexer.next();
            if (current.type == TokenType::Number) {
                z = parseNumber(current.text);
            }
        }
        
        current = lexer.next();
    }
    
    return Vector3(x, y, z);
}

SimDef SimulationBuilder::SceneParsing(const std::string& text, const std::string& name) {
    Lexer lexer(text);
    SimDef simDef;
    Token current;

    current = lexer.next();
    if (current.type == TokenType::Identifier && current.text == "World") {
        simDef.name = name;
        current = lexer.next();
        if (current.type == TokenType::OBrace) {
            current = lexer.next();
            while (current.type != TokenType::CBrace && current.type != TokenType::EndOfFile) {
                if (current.type == TokenType::Identifier) {
                    Elements elem;
                    elem.id = current.text;   
                    current = lexer.next();
                    if (current.type == TokenType::LBrace) {
                        current = lexer.next();
                        while (current.type != TokenType::RBrace && current.type != TokenType::EndOfFile) {
                            if (current.type == TokenType::Identifier) {
                                std::string keyword = current.text;
                                
                                if (keyword == "pos") {
                                    elem.parameters["position"] = parseVector3(lexer);
                                } else if (keyword == "color") {
                                    elem.parameters["color"] = parseVector3(lexer);   
                                } else if (keyword == "RotSpeed") {
                                    elem.parameters["RotSpeed"] = parseVector3(lexer);
                                } 
                            } else if (current.type == TokenType::Number) {
                                
                                if (elem.id == "Star") {
                                    if (!elem.parameters.count("mass")) {
                                        elem.parameters["mass"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("radius")) {
                                        elem.parameters["radius"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("luminosity")) {
                                        elem.parameters["luminosity"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("temperature")) {
                                        elem.parameters["temperature"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Planet") {
                                    if (!elem.parameters.count("mass")) {
                                        elem.parameters["mass"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("radius")) {
                                        elem.parameters["radius"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("AxialTilt")) {
                                        elem.parameters["AxialTilt"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Satellite") {
                                    if (!elem.parameters.count("mass")) {
                                        elem.parameters["mass"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("radius")) {
                                        elem.parameters["radius"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Cube") {
                                    if (!elem.parameters.count("size"))
                                    {
                                        elem.parameters["size"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Plane") {
                                    if (!elem.parameters.count("width"))
                                    {
                                        elem.parameters["width"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("height"))
                                    {
                                        elem.parameters["height"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Sphere") {
                                    if (!elem.parameters.count("radius")) {
                                        elem.parameters["radius"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("sectorCount")) {
                                        elem.parameters["sectorCount"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("stackCount")) {
                                        elem.parameters["stackCount"] = parseNumber(current.text);
                                    }
                                } else if (elem.id == "Light") {
                                    if (!elem.parameters.count("radius")) {
                                        elem.parameters["radius"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("sectorCount")) {
                                        elem.parameters["sectorCount"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("stackCount")) {
                                        elem.parameters["stackCount"] = parseNumber(current.text);
                                    } else if (!elem.parameters.count("intensity")) {
                                        elem.parameters["intensity"] = parseNumber(current.text);
                                    }
                                }
                            }
                            current = lexer.next();
                            if (current.type == TokenType::Comma) {
                                current = lexer.next();
                            }
                        }
                        simDef.elements.push_back(elem);
                    }
                }
                current = lexer.next();
            }
        }
    }
    return simDef;
}

std::vector<SimDef>  SimulationBuilder::SceneLoadFile(){
    std::vector<SimDef> ListSimDef;
    fs::path scenepath = path;

    // On vérifie si le dossier existe pour éviter un crash
    if (!fs::exists(scenepath) || !fs::is_directory(scenepath)) return ListSimDef;

    for (const auto& file : fs::directory_iterator(scenepath)) {
        const auto& fpath = file.path();

        // On ne veut que les fichiers de simulation .wrld
        if (fpath.extension() == ".wrld") {
            std::ifstream ifs(file.path());
            std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
            SimDef sd = SceneParsing(content, fpath.stem().string()); // Récupération contenu fichier puis nom fichier
            ListSimDef.push_back(sd);
        }
    }

    return ListSimDef;
}

World SimulationBuilder::SceneBuilder(SimDef simDef) {
    World world;
    world.sceneName = simDef.name;
    const float G = 6.674e-3f; // pour calcul de la Vitesse 6.674e-3f

    // Trouver le corps central (le plus massif)
    float maxMass = 0.0f;
    Vector3 centralPos = Vector3::Zero;
    for (const auto& elem : simDef.elements) {
        auto it = elem.parameters.find("mass");
        if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
            float m = std::get<float>(it->second);
            if (m > maxMass) {
                maxMass = m;
                auto posIt = elem.parameters.find("position");
                if (posIt != elem.parameters.end() && std::holds_alternative<Vector3>(posIt->second)) {
                    centralPos = std::get<Vector3>(posIt->second);
                }
            }
        }
    }

    for (const auto& elem : simDef.elements) {
        
        if (elem.id == "Star") {
            Vector3 position = Vector3::Zero;
            float mass = 1.0f;
            float radius = 1.0f;
            Vector3 color = Vector3::Zero;
            float luminosity = 1.0f;
            float temperature = 5000.0f;
            Vector3 velocity;
            
            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("mass");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                mass = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("radius");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                radius = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("color");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                color = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("luminosity");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                luminosity = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("temperature");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                temperature = std::get<float>(it->second);
            }

            // Lumiere selon la température
            if (color == Vector3::Zero) {
                color = Star::TemperatureColor(temperature);
            }else
            {
                if (temperature < Star::GetTemperatureColor(color) - 200 || temperature > Star::GetTemperatureColor(color) + 200 ) {
                    temperature = Star::GetTemperatureColor(color);
                }
            }
            
            // vitesse orbitale selon kepler
            Vector3 toCenter = centralPos - position;
            float distance = std::sqrt(toCenter.Dot(toCenter));
            if (distance > 0.001f && maxMass > mass) {  // pas le corps central lui-même
                float vOrb = std::sqrt((G * maxMass) / distance);
                // vecteur perpendiculaire à toCenter dans le plan XZ
                Vector3 perpendicular(-toCenter.z, 0.0f, toCenter.x);
                perpendicular = perpendicular.Normalisation();
                velocity = perpendicular * vOrb;
            } else {
                velocity = Vector3::Zero;  // le corps central ne bouge pas (ou très peu)
            }
            
            world.addStar(position, mass, radius, luminosity, color, temperature, velocity);
        }
        
        else if (elem.id == "Planet") {
            Vector3 position = Vector3::Zero;
            float mass = 1.0f;
            float radius = 1.0f;
            Vector3 color = Vector3::One;
            Vector3 velocity = Vector3::Zero;
            Vector3 RotSpeed = Vector3::Zero;
            float axialTilt = 0.0f;
            
            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("mass");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                mass = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("radius");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                radius = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("color");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                color = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("RotSpeed");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                RotSpeed = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("AxialTilt");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                axialTilt = std::get<float>(it->second);
            }

            // vitesse orbitale selon kepler
            Vector3 toCenter = centralPos - position;
            float distance = std::sqrt(toCenter.Dot(toCenter));
            if (distance > 0.001f && maxMass > mass) {  // pas le corps central lui-même
                float vOrb = std::sqrt((G * maxMass) / distance);
                // vecteur perpendiculaire à toCenter dans le plan XZ
                Vector3 perpendicular(-toCenter.z, 0.0f, toCenter.x);
                perpendicular = perpendicular.Normalisation();
                velocity = perpendicular * vOrb;
            } else {
                velocity = Vector3::Zero;  // le corps central ne bouge pas (ou très peu)
            }
            
            world.addPlanet(position, mass, radius, color, RotSpeed, axialTilt, velocity);
        }
        
        else if (elem.id == "Satellite") {
            Vector3 position = Vector3::Zero;
            float mass = 1.0f;
            float radius = 1.0f;
            Vector3 color = Vector3::One;
            Vector3 velocity;
            
            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }
            
            it = elem.parameters.find("mass");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                mass = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("radius");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                radius = std::get<float>(it->second);
            }
            
            it = elem.parameters.find("color");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                color = std::get<Vector3>(it->second);
            }

            // vitesse orbitale selon kepler
            Vector3 toCenter = centralPos - position;
            float distance = std::sqrt(toCenter.Dot(toCenter));
            if (distance > 0.001f && maxMass > mass) {  // pas le corps central lui-même
                float vOrb = std::sqrt((G * maxMass) / distance);
                // vecteur perpendiculaire à toCenter dans le plan XZ
                Vector3 perpendicular(-toCenter.z, 0.0f, toCenter.x);
                perpendicular = perpendicular.Normalisation();
                velocity = perpendicular * vOrb;
            } else {
                velocity = Vector3::Zero;  // le corps central ne bouge pas ou très peu
            }
            
            world.addSatellite(position, mass, radius, color, velocity);
        } else if (elem.id == "Cube") {
            float size;
            Vector3 position;

            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("size");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                size = std::get<float>(it->second);
            }

            world.addCube(size, position);
        } else if (elem.id == "Plane") {
            Vector3 position;
            float width;
            float height;

            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("width");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                width = std::get<float>(it->second);
            }

            it = elem.parameters.find("height");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                height = std::get<float>(it->second);
            }

            world.addPlane(width, height, 1, 1, position);
        } else if (elem.id == "Sphere") {
            Vector3 position;
            Vector3 color;
            float radius;
            float sectorCount;
            float stackCount;

            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("color");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                color = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("radius");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                radius = std::get<float>(it->second);
            }

            it = elem.parameters.find("stackCount");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                stackCount = std::get<float>(it->second);
            }

            it = elem.parameters.find("sectorCount");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                sectorCount = std::get<float>(it->second);
            }

            world.addSphereSolid(radius, sectorCount, stackCount, position, color);
        } else if (elem.id == "Light") {
            Vector3 position;
            Vector3 color;
            float radius;
            float sectorCount;
            float stackCount;
            float intensity;

            auto it = elem.parameters.find("position");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                position = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("color");
            if (it != elem.parameters.end() && std::holds_alternative<Vector3>(it->second)) {
                color = std::get<Vector3>(it->second);
            }

            it = elem.parameters.find("radius");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                radius = std::get<float>(it->second);
            }

            it = elem.parameters.find("stackCount");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                stackCount = std::get<float>(it->second);
            }

            it = elem.parameters.find("sectorCount");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                sectorCount = std::get<float>(it->second);
            }

            it = elem.parameters.find("intensity");
            if (it != elem.parameters.end() && std::holds_alternative<float>(it->second)) {
                intensity = std::get<float>(it->second);
            }

            world.addSphereLight(radius, sectorCount, stackCount, position, color, intensity);
        }
    }

    return world;
}

SimulationManager::SimulationManager(World& world) {
    std::cout << " DEBUG: Démarrage SimulationManager" << std::endl;
    std::cout << " Chemin actuel: " << fs::current_path() << std::endl;
    std::cout << " Chemin world: " << path << std::endl;
    std::cout << " Dossier existe ? " << (fs::exists(path) ? "OUI" : "NON") << std::endl;
    
    simDefs = builder.SceneLoadFile();
    
    std::cout << " Fichiers .wrld trouvés: " << simDefs.size() << std::endl;
    
    // VÉRIFICATION CRITIQUE
    if (simDefs.empty()) {
        std::cout << " ERREUR: Aucun fichier .wrld trouvé!" << std::endl;
        std::cout << " Création d'un monde par défaut..." << std::endl;
        
        // Créer un monde de secours
        World defaultWorld;
        defaultWorld.sceneName = "DefaultWorld";
        defaultWorld.addStar(Vector3(0, 0, -10), 12.0f, 5.0f, 5.0f, Vector3(1.0f, 0.0f, 0.5f), 5000.0f, Vector3::Zero);
        worlds.push_back(std::move(defaultWorld));
        
        // Charger ce monde
        world = worlds[0];  // Copie directe
        CurrentWorld = "DefaultWorld";
        return;
    }
    
    // Construction normale
    for (const auto& simDef : simDefs) {
        std::cout << " Construction: " << simDef.name << std::endl;
        World newWorld = builder.SceneBuilder(simDef);
        worlds.push_back(std::move(newWorld));
    }
    
    // VÉRIFICATION avant d'accéder à worlds[0]
    if (!worlds.empty()) {
        std::cout << " Chargement du premier monde..." << std::endl;
        LoadSimulation(world, worlds[0].sceneName);
    }
}

SimulationManager::~SimulationManager() {
    simDefs.clear();
    worlds.clear();
}

void SimulationManager::LoadSimulation(World& world, const std::string& wname) {
    for (const auto& wrld : worlds) {
        if (wrld.sceneName == wname)
        {
            world = wrld;
            CurrentWorld = wname;
            break;
        }
    }
}

void SimulationManager::UpdateSimulationList() {
    for (const auto& file : fs::directory_iterator(path)) { 
        if ((fs::last_write_time(file) > last_modif)) {
            if (file.path().extension() == ".wrld") {
                std::ifstream ifs(file.path());
                std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
                SimDef sd = builder.SceneParsing(content, file.path().stem().string());
                bool find = false;
                for (auto& simdef : simDefs)
                {
                    if(simdef.name == sd.name) {
                        simdef = sd;
                        find = true;
                    } 
                }
                if (!find) {
                    simDefs.push_back(sd);
                    worlds.push_back(builder.SceneBuilder(sd));
                }
            }
        }
    }
}

std::vector<std::string> SimulationManager::GetSceneNames() const {
    std::vector<std::string> names;
    for (const auto& world : worlds) {
        names.push_back(world.sceneName);
    }
    return names;
}

bool SimulationManager::SceneExists(const std::string& sceneName) const {
    for (const auto& world : worlds) {
        if (world.sceneName == sceneName) {
            return true;
        }
    }
    return false;
}

bool SimulationManager::CreateNewSimulation(const std::string& sceneName) {
    // Vérifier que le nom n'existe pas déjà
    if (SceneExists(sceneName)) {
        std::cout << " ERREUR: Une scène nommée '" << sceneName << "' existe déjà!" << std::endl;
        return false;
    }
    
    // Créer un nouveau monde vide
    World newWorld;
    newWorld.sceneName = sceneName;
    
    // Ajouter quelques objets par défaut
    // newWorld.addStar(Vector3::Zero, 100.0f, 10.0f, 50.0f, Vector3(1.0f, 1.0f, 0.8f), 5778.0f, Vector3::Zero);
    
    // Créer la définition de simulation correspondante
    SimDef newSimDef;
    newSimDef.name = sceneName;
    
    // Ajouter aux listes
    simDefs.push_back(newSimDef);
    worlds.push_back(std::move(newWorld));
    
    // Sauvegarder dans un fichier .wrld
    SaveSimulationToFile(sceneName);
    
    std::cout << " Nouvelle scène créée: " << sceneName << std::endl;
    return true;
}

bool SimulationManager::SaveCurrentSimulation() {
    if (SaveSimulationToFile(CurrentWorld)) return true;
    return false;
}

bool SimulationManager::SaveSimulationToFile(const std::string& sceneName) {
    // Trouver la scène dans worlds
    World* targetWorld = nullptr;
    for (auto& world : worlds) {
        if (world.sceneName == sceneName) {
            targetWorld = &world;
            break;
        }
    }
    
    if (!targetWorld) {
        std::cout << " Scène non trouvée: " << sceneName << std::endl;
        return false;
    }
    fs::path filepath = path / (sceneName + ".wrld");
    
    // Ouvrir le fichier en écriture
    std::ofstream file(filepath);
    if (!file.is_open()) {
        std::cout << " Impossible de créer le fichier: " << filepath << std::endl;
        return false;
    }
    
    // Écrire la structure du fichier .wrld
    file << "World {\n";
    // Parcourir tous les objets de la scène
    for (const auto& obj : targetWorld->getObjects()) {
        // Essayer de cast en Astre pour récupérer les propriétés
        Astre* astre = dynamic_cast<Astre*>(obj.get());
        
        if (astre) {
            Star* star = dynamic_cast<Star*>(astre);
            planete* planet = dynamic_cast<planete*>(astre);
            Satellite* satellite = dynamic_cast<Satellite*>(astre);
            
            if (star) {
                // Sauvegarder une étoile
                file << "    Star(" 
                     << star->mass << ","
                     << star->radius << ","
                     << star->luminosity << ","
                     << star->temperature << ",";
                file << "pos(" << star->transform.position.x << ", " 
                     << star->transform.position.y << ", " 
                     << star->transform.position.z << ") ";
                file << "color(" << star->material.emissiveColor.x << ", "
                     << star->material.emissiveColor.y << ", "
                     << star->material.emissiveColor.z << "))\n";
                     
            } else if (planet) {
                // Sauvegarder une planète
                file << "    Planet("
                     << planet->mass << ","
                     << planet->radius << ","
                     << planet->AxialTilt << ",";
                file << "pos(" << planet->transform.position.x << ", "
                     << planet->transform.position.y << ", "
                     << planet->transform.position.z << ") ";
                file << "color(" << planet->material.albedoColor.x << ", "
                     << planet->material.albedoColor.y << ", "
                     << planet->material.albedoColor.z << ") ";
                file << "RotSpeed(" << planet->rotationSpeed.x << ", "
                     << planet->rotationSpeed.y << ", "
                     << planet->rotationSpeed.z << "))\n";
            } else if (satellite) {
                // Sauvegarder un satellite
                file << "    Satellite("
                     << satellite->mass << ","
                     << satellite->radius << ",";
                file << "pos(" << satellite->transform.position.x << ", "
                     << satellite->transform.position.y << ", "
                     << satellite->transform.position.z << ") ";
                file << "color(" << satellite->material.albedoColor.x << ", "
                     << satellite->material.albedoColor.y << ", "
                     << satellite->material.albedoColor.z << "))\n";
            }
        }
    }   
    file << "}\n";
    file.close();
    
    std::cout << " Scène sauvegardée: " << filepath << std::endl;
    return true;
}

bool SimulationManager::DeleteSimulation(const std::string& sceneName) {
    // Ne pas supprimer la scène actuelle
    if (sceneName == CurrentWorld) {
        std::cout << " Impossible de supprimer la scène active!" << std::endl;
        return false;
    }
    
    // Supprimer de la liste worlds
    auto worldIt = std::find_if(worlds.begin(), worlds.end(), [&sceneName](const World& w) { return w.sceneName == sceneName; });
    if (worldIt != worlds.end()) {
        worlds.erase(worldIt);
    }
    
    // Supprimer de la liste simDefs
    auto simDefIt = std::find_if(simDefs.begin(), simDefs.end(),
        [&sceneName](const SimDef& sd) { return sd.name == sceneName; });
    
    if (simDefIt != simDefs.end()) {
        simDefs.erase(simDefIt);
    }
    
    // Supprimer le fichier .wrld
    fs::path filepath = path / (sceneName + ".wrld");
    
    if (fs::exists(filepath)) {
        fs::remove(filepath);
        std::cout << " Fichier supprimé: " << filepath << std::endl;
    }
    
    std::cout << " Scène supprimée: " << sceneName << std::endl;
    return true;
}

void SimulationManager::RefreshSceneList(World& world) {
    std::cout << " Rafraîchissement de la liste des scènes..." << std::endl;
    
    // Sauvegarder la scène actuelle
    std::string currentScene = CurrentWorld;
    
    // Vider les listes
    simDefs.clear();
    worlds.clear();
    
    // Recharger depuis les fichiers
    simDefs = builder.SceneLoadFile();
    
    for (const auto& simDef : simDefs) {
        World newWorld = builder.SceneBuilder(simDef);
        worlds.push_back(std::move(newWorld));
    }
    
    // Recharger la scène qui était active
    if (SceneExists(currentScene)) {
        LoadSimulation(world, currentScene);
    } else if (!worlds.empty()) {
        LoadSimulation(world, worlds[0].sceneName);
    }
    std::cout << " Liste rafraîchie: " << worlds.size() << " scène(s) trouvée(s)" << std::endl;
}