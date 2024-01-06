#include <stdio.h>
#include <math.h>

// Définition des constantes
const double lambda = 0.1; // Constante d'amortissement
const double w0 = 1.0;     // Fréquence naturelle
const double w = 0.1;      // Fréquence de forçage
const double amplitude = 1.0; // Amplitude du signal carré
const double offset = 0.0;   // Décalage temporel du premier créneau
const double width = 100.0;   // Largeur des créneaux

// Fonction pour calculer la dérivée de theta
double dTheta(double omega) {
    return omega;
}

// Fonction pour calculer la dérivée de omega avec un signal carré
double dOmega(double theta, double omega, double t) {
    // Utilisation d'un signal carré avec une fréquence w, un décalage temporel offset,
    // et une largeur width
    double squareSignal = fmod((t - offset), width) < width / 2.0 ? amplitude : -amplitude;

    return -2 * lambda * omega - w0 * w0 * theta + squareSignal;
}
// Fonction principale
int main() {
    FILE *file = fopen("phasec2.dat", "w"); // Ouvre le fichier en écriture

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier phasec2.dat\n");
        return 1; // Quitte le programme avec un code d'erreur
    }

    double theta = 0.0; // Condition initiale pour theta
    double omega = 2.0; // Condition initiale pour omega
    double t = 0.0;     // Temps initial
    double dt = 0.01;   // Pas de temps
    int steps = 50000;   // Nombre de pas

    for (int i = 0; i < steps; i++) {
        // Calcul des k et j pour Runge-Kutta
        double k1 = dTheta(omega) * dt;
        double j1 = dOmega(theta, omega, t) * dt;

        double k2 = dTheta(omega + j1 / 2) * dt;
        double j2 = dOmega(theta + k1 / 2, omega + j1 / 2, t + dt / 2) * dt;

        double k3 = dTheta(omega + j2 / 2) * dt;
        double j3 = dOmega(theta + k2 / 2, omega + j2 / 2, t + dt / 2) * dt;

        double k4 = dTheta(omega + j3) * dt;
        double j4 = dOmega(theta + k3, omega + j3, t + dt) * dt;

        // Mise à jour de theta et omega
        theta += (k1 + 2 * k2 + 2 * k3 + k4) / 6;
        omega += (j1 + 2 * j2 + 2 * j3 + j4) / 6;
        t += dt;

        // Écriture dans le fichier
        fprintf(file, "%f\t%f\t%f\n", t, theta, omega);
    }

    fclose(file); // Ferme le fichier

    return 0;
}