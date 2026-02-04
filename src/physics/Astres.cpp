#include "Astres.h"
#include <algorithm>

const Vector3 Star::TemperatureColor(float temp) {
    // clamp de la valeur avec un min de 1000k et 40000k division par 100 pour travailler avec de valeurs agréables
    float t = std::clamp(temp, 1000.0f, 40000.0f) / 100;
    float r;
    float g;
    float b;
    
    // Red and green
    if (t <= 66)
    {
        r = 1.0f; 
        g = (99.4708025861 * std::log(t) - 161.1195681661) / 255.0f;
    } else if (t > 66)
    {
        r = (329.698727446 * std::pow((t-60),-0.1332047592)) / 255.0f;
        g = (288.1221695283 * (std::pow(t - 60, -0.0755148492))) / 255.0f;
    }
    
    // Blue
    if (t >= 66)
    {
        b = 0.0f;
    } else if (t <= 19)
    {
        b = 0.0f;
    }else
    {
        b = (138.5177312231 * std::log(t - 10) - 305.0447927307) / 255.0f;
    }
    
    r = std::clamp(r, 0.0f, 1.0f);
    g = std::clamp(g, 0.0f, 1.0f);
    b = std::clamp(b, 0.0f, 1.0f);

    return Vector3(r, g, b);
}

float Star::GetTemperatureColor(const Vector3& col) {
    float r = col.x;
    float g = col.y;
    float b = col.z;

    // 1. Normalisation pour obtenir les coordonnées de chromaticité x, y
    float total = r + g + b;
    if (total == 0) return 0.0f;

    float x = r / total;
    float y = g / total;

    // 2. Calcul du facteur n de McCamy
    // Le point de référence (0.3320, 0.1858) est lié au "Locus de Planck"
    float n = (x - 0.3320f) / (y - 0.1858f);

    // 3. Formule polynomiale de McCamy
    // T = -449n³ + 3525n² - 6823.3n + 5524.33
    float n2 = n * n;
    float n3 = n2 * n;

    float temperature = -449.0f * n3 + 3525.0f * n2 - 6823.3f * n + 5524.33f;

    return temperature;
    
}