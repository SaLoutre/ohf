#include <stdio.h>
#include <math.h>
#include <complex.h>

const double lambda = 0.1;  // Constante d'amortissement
const double w0 = 1.0;      // Fréquence naturelle
const double w_start = 0.2; // Fréquence de forçage de départ
const double w_end = 3.0;   // Fréquence de forçage de fin
const double w_step = 0.05; // Pas de fréquence de forçage
const double dt = 0.01;     // Pas de temps
const int transient_steps = 1000; // Nombre de pas pour atteindre le régime stationnaire
const int steps_per_frequency = 100; // Nombre de pas pour calculer l'amplitude

double dTheta(double omega) {
    return omega;
}

double dOmega(double theta, double omega, double w, double t) {
    return -2 * lambda * omega - w0 * w0 * theta + sin(w * t);
}

int main() {
    FILE *file = fopen("decal.dat", "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier decal.dat\n");
        return 1;
    }

    for (double w = w_start; w <= w_end; w += w_step) {
        double theta = 0.0;
        double omega = 2.0;
        double t = 0.0;
        double max_amplitude = 0.0;

        // Transient phase - permet au système d'atteindre un état stationnaire
        for (int i = 0; i < transient_steps; ++i) {
            // ... (comme dans votre code existant)
        }

        // Measurement phase - mesure de l'amplitude et du décalage
        for (int i = 0; i < steps_per_frequency; ++i) {
            // ... (comme dans votre code existant)

            if (fabs(theta) > max_amplitude) {
                max_amplitude = fabs(theta);
            }
        }

        // Calcul du décalage phi
        double phi = atan2(2 * lambda * w, w0 * w0 - w * w);

        // Écriture de la fréquence, de l'amplitude maximale et du décalage dans le fichier
        fprintf(file, "%f\t%f\n", w, phi);
    }

    fclose(file);
    printf("Simulation terminée. Les données de décalage sont enregistrées dans 'decal.dat'.\n");

    return 0;
}