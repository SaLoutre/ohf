#include <stdio.h>
#include <math.h>

// Définition des constantes
const double lambda = 0.1; // Constante d'amortissement
const double w0 = 1.0;    // Fréquence naturelle
const double w = 0.1;     // Fréquence de forçage
const double diracFrequency = 10000.0; // Fréquence du peigne de Dirac

// Fonction pour calculer la dérivée de theta
double dTheta(double omega) {
    return omega;
}

// Fonction pour calculer la dérivée de omega avec un peigne de Dirac
double dOmega(double theta, double omega, double t) {
    // Utilisation d'un peigne de Dirac avec une fréquence diracFrequency
    double diracSignal = 0.0;

    // Ajout d'une impulsion de Dirac à chaque intervalle régulier
    if (fmod(t, 1.0 / diracFrequency) < 1e-10) {
        diracSignal = 10.0;
    }

    return -2 * lambda * omega - w0 * w0 * theta + diracSignal;
}

// Fonction principale
int main() {
    FILE *file = fopen("phasedirac.dat", "w"); // Ouvre le fichier en écriture

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier phasedirac.dat\n");
        return 1; // Quitte le programme avec un code d'erreur
    }

    double theta = 0.0; // Condition initiale pour theta
    double omega = 2.0; // Condition initiale pour omega
    double t = 0.0;     // Temps initial
    double dt = 0.01;   // Pas de temps
    int steps = 20000;   // Nombre de pas
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