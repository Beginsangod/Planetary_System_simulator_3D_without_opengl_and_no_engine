#pragma once
#include "../core/math/Vector3.h"
#include "../physics/world.h"
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <fstream>
#include <filesystem>

// stocke liste de type de Variable
typedef std::variant<int, float, std::string, bool, Vector3> Variant;

// Eléments du mondes
struct Elements {
    std::string id;
    std::unordered_map<std::string, Variant> parameters;
};

// Patron de monde pour les fichiers .wrld
typedef struct SimulationDefinition
{
    std::string name;
    std::vector<Elements> elements;
} SimDef; 

// type de token
enum class TokenType {
    Identifier,
    Number,
    LBrace,     // (
    RBrace,     // )
    OBrace,     // {
    CBrace,     // }
    Colon,      // :
    Comma,      // ,
    EndOfFile
};

// Type Token
struct Token {
    TokenType type;
    std::string text;
};

// analyseur grammaticale
class Lexer {
    private:
        char peek();
        char advance();
        void skipWhitespace();

        std::string source;
        size_t pos = 0;
    public:
        Lexer(const std::string& src):source(src) {};
        Token next();
        TokenType peekType() {
            size_t startPos = pos;
            Token t = next();
            pos = startPos; 
            return t.type;
        }
};

// Builer de monde
class SimulationBuilder {
    public:

        SimDef SceneParsing(const std::string& text, const std::string& name);
        std::vector<SimDef> SceneLoadFile();
        World SceneBuilder(SimDef simDef);
};

// Manager des scènes du simulateur
class SimulationManager {
    private:
        std::vector<SimDef> simDefs;
        std::vector<World> worlds;
        SimulationBuilder builder;
        std::string CurrentWorld;

    public:
        SimulationManager(World& world);
        ~SimulationManager();

        void LoadSimulation(World& world,const std::string& wname);
        void UpdateSimulationList();
        std::string GetCurrentSimulation() { return CurrentWorld; };
        std::vector<std::string> GetSceneNames() const;
        int GetSceneCount() const { return worlds.size(); }
        bool SceneExists(const std::string& sceneName) const;
        bool CreateNewSimulation(const std::string& sceneName);
        bool SaveSimulationToFile(const std::string& sceneName);
        // Supprimer une scène (ne peut pas supprimer la scène actuelle)
        bool DeleteSimulation(const std::string& sceneName);
        
        // Sauvegarder la scène actuelle dans un fichier .wrld
        bool SaveCurrentSimulation();
        
        // Rafraîchir la liste des scènes depuis le dossier
        void RefreshSceneList(World& world);
};

