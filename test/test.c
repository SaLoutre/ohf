#include <stdio.h>
#include <math.h>

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
    FILE *file = fopen("resonance.dat", "w");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier resonance.dat\n");
        return 1;
    }

    for (double w = w_start; w <= w_end; w += w_step) {
        double theta = 0.0;
        double omega = 2.0;
        double t = 0.0;
        double max_amplitude = 0.0;

        // Transient phase - permet au système d'atteindre un état stationnaire
        for (int i = 0; i < transient_steps; ++i) {
            double k1 = dTheta(omega) * dt;
            double j1 = dOmega(theta, omega, w, t) * dt;

            double k2 = dTheta(omega + j1 / 2) * dt;
            double j2 = dOmega(theta + k1 / 2, omega + j1 / 2, w, t + dt / 2) * dt;

            double k3 = dTheta(omega + j2 / 2) * dt;
            double j3 = dOmega(theta + k2 / 2, omega + j2 / 2, w, t + dt / 2) * dt;

            double k4 = dTheta(omega + j3) * dt;
            double j4 = dOmega(theta + k3, omega + j3, w, t + dt) * dt;

            theta += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
            omega += (j1 + 2 * j2 + 2 * j3 + j4) / 6;
            t += dt;
        }
        // Measurement phase - mesure de l'amplitude dans le régime stationnaire
        for (int i = 0; i < steps_per_frequency; ++i) {
            double k1 = dTheta(omega) * dt;
            double j1 = dOmega(theta, omega, w, t) * dt;

            double k2 = dTheta(omega + j1 / 2) * dt;
            double j2 = dOmega(theta + k1 / 2, omega + j1 / 2, w, t + dt / 2) * dt;

            double k3 = dTheta(omega + j2 / 2) * dt;
            double j3 = dOmega(theta + k2 / 2, omega + j2 / 2, w, t + dt / 2) * dt;

            double k4 = dTheta(omega + j3) * dt;
            double j4 = dOmega(theta + k3, omega + j3, w, t + dt) * dt;

            theta += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
            omega += (j1 + 2 * j2 + 2 * j3 + j4) / 6;
            t += dt;

            if (fabs(theta) > max_amplitude) {
                max_amplitude = fabs(theta);
            }
        }

        // Écriture de la fréquence et de l'amplitude maximale dans le fichier
        fprintf(file, "%f\t%f\n", w, max_amplitude);
    }

    fclose(file);
    printf("Simulation terminée. Les données de résonance sont enregistrées dans 'resonance.dat'.\n");

    return 0;
}