#include "MotorPhysics.h"

void MotorPhysics::Update(float fdeltaTime){
    float deltaTime = fdeltaTime * 2;
    // Récupération des Astres de la scène 
    std::vector<Astre*> allAstres;

    for (auto& obj : world.getObjects()) {
        Astre* a = dynamic_cast<Astre*>(obj.get());
        if (a) allAstres.push_back(a);
    }
    int countastre = allAstres.size(); // Compter les Astres de la scène
    if (countastre < 2) return; // Pas de calcul si moins de 2 Astres
    float eps2 = epsoftening * epsoftening;

    // Mise à 0 de l'accélération de tout les Astres
    for (auto& Astre : allAstres) {
        Astre->acceleration = Vector3(0, 0, 0);
    }

    for(int i = 0; i < countastre; i++){
        for (int j = i + 1; j < countastre; j++)
        {
            Vector3 ri = allAstres[i]->transform.position;
            Vector3 rj = allAstres[j]->transform.position;

            // calcul de l'inverse de la distance séparant les planètes et de la direction de la Force
            Vector3 delta = rj - ri;
            float dist2 = delta.Dot(delta) + eps2;
            float invDist = 1/std::sqrt(dist2);
            float invDist3 = invDist * invDist * invDist;
            
            Vector3 forceDirection = delta * invDist3;

            allAstres[i]->acceleration += forceDirection * g * allAstres[j]->mass;
            allAstres[j]->acceleration += forceDirection * -g * allAstres[i]->mass;
        }
    }

    // Velocity Verlet – demi-pas vitesse + position
    for (auto& astre : allAstres)
    {
        astre->velocity += astre->acceleration * (0.5 * deltaTime);
        astre->transform.position += (astre->velocity * deltaTime);
    }
    
    for (auto& Astre : allAstres) {
        Astre->acceleration = Vector3(0, 0, 0);
    }

    for(int i = 0; i < countastre; i++){
        for (int j = i + 1; j < countastre; j++)
        {
            Vector3 ri = allAstres[i]->transform.position;
            Vector3 rj = allAstres[j]->transform.position;

            // calcul de l'inverse de la distance séparant les planètes et de la direction de la Force
            Vector3 delta = rj - ri;
            float dist2 = delta.Dot(delta) + eps2;
            float invDist = 1/std::sqrt(dist2);
            float invDist3 = invDist * invDist * invDist;
            
            Vector3 forceDirection = delta * invDist3;

            allAstres[i]->acceleration += forceDirection * g * allAstres[j]->mass;
            allAstres[j]->acceleration += forceDirection * -g * allAstres[i]->mass;
        }
    }

    for (auto& astre : allAstres)
    {
        astre->velocity += astre->acceleration * (0.5 * deltaTime);
    };

    EliminateCollisions(allAstres);
}

void MotorPhysics::EliminateCollisions(std::vector<Astre*>& astres) {
    int count = astres.size();

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            Vector3 delta = astres[j]->transform.position - astres[i]->transform.position;
            float dist2 = delta.Dot(delta);
            float sumRadii = astres[i]->radius + astres[j]->radius;

            // Pas de collision
            if (dist2 >= sumRadii * sumRadii || dist2 < 0.0001f) continue;

            float dist = std::sqrt(dist2);
            Vector3 normal = delta * (1.0f / dist);
            float totalMass = astres[i]->mass + astres[j]->mass;

            // Séparer les deux corps selon leur masse
            float overlap = sumRadii - dist;
            astres[i]->transform.position -= normal * (overlap * astres[j]->mass / totalMass);
            astres[j]->transform.position += normal * (overlap * astres[i]->mass / totalMass);

            // Résoudre la vitesse uniquement s'ils se rapprochent
            Vector3 relVel = astres[j]->velocity - astres[i]->velocity;
            float velDotNormal = relVel.Dot(normal);

            if (velDotNormal < 0.0f) {
                float impulse = -1.5f * velDotNormal / totalMass;
                astres[i]->velocity -= normal * (impulse * astres[j]->mass);
                astres[j]->velocity += normal * (impulse * astres[i]->mass);
            }
        }
    }
}