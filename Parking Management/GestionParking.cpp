// ============================================================================
// PROJET COMPLET DE GESTION DE PARKING - FICHIER UNIQUE
// Module 9 - Véhicules et Parking
// Tous les concepts POO en C++ : Héritage, Polymorphisme, STL, Exceptions
// ============================================================================

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <string>
#include <chrono>
#include <iomanip>
#include <exception>
#include <algorithm>
#include <windows.h>

using namespace std;

// ============================================================================
// CLASSE DE BASE : Vehicle (abstraite)
// ============================================================================

class Vehicle {
protected:
    string matricule;
    string marque;
    string modele;
    string couleur;
    int annee;

public:
    // Constructeur
    Vehicle(const string& mat, const string& marq,
        const string& mod, const string& coul, int an)
        : matricule(mat), marque(marq), modele(mod), couleur(coul), annee(an) {
    }

    // Destructeur virtuel
    virtual ~Vehicle() {}

    // Getters
    string getMatricule() const { return matricule; }
    string getMarque() const { return marque; }
    string getModele() const { return modele; }
    string getCouleur() const { return couleur; }
    int getAnnee() const { return annee; }

    // Méthodes virtuelles pures (polymorphisme)
    virtual string getType() const = 0;
    virtual double getTarifHoraire() const = 0;
    virtual void afficher() const = 0;

    // Surcharge d'opérateurs
    friend ostream& operator<<(ostream& os, const Vehicle& v) {
        os << "Type: " << v.getType() << "\n"
            << "Matricule: " << v.matricule << "\n"
            << "Marque: " << v.marque << "\n"
            << "Modele: " << v.modele << "\n"
            << "Couleur: " << v.couleur << "\n"
            << "Annee: " << v.annee << "\n"
            << "Tarif horaire: " << v.getTarifHoraire() << " DH";
        return os;
    }

    bool operator==(const Vehicle& other) const {
        return matricule == other.matricule;
    }
};

// ============================================================================
// CLASSE DÉRIVÉE : Voiture
// ============================================================================

class Voiture : public Vehicle {
private:
    int nombrePortes;
    string typeCarburant;

public:
    Voiture(const string& mat, const string& marq,
        const string& mod, const string& coul, int an,
        int nbPortes, const string& carburant)
        : Vehicle(mat, marq, mod, coul, an), nombrePortes(nbPortes),
        typeCarburant(carburant) {
    }

    int getNombrePortes() const { return nombrePortes; }
    string getTypeCarburant() const { return typeCarburant; }

    string getType() const override { return "Voiture"; }
    double getTarifHoraire() const override { return 10.0; }

    void afficher() const override {
        cout << *this << "\n"
            << "Nombre de portes: " << nombrePortes << "\n"
            << "Type de carburant: " << typeCarburant << endl;
    }
};

// ============================================================================
// CLASSE DÉRIVÉE : Moto
// ============================================================================

class Moto : public Vehicle {
private:
    int cylindree;
    bool avecSidecar;

public:
    Moto(const string& mat, const string& marq,
        const string& mod, const string& coul, int an,
        int cyl, bool sidecar = false)
        : Vehicle(mat, marq, mod, coul, an), cylindree(cyl), avecSidecar(sidecar) {
    }

    int getCylindree() const { return cylindree; }
    bool hasAvecSidecar() const { return avecSidecar; }

    string getType() const override { return "Moto"; }
    double getTarifHoraire() const override { return 5.0; }

    void afficher() const override {
        cout << *this << "\n"
            << "Cylindree: " << cylindree << " cc\n"
            << "Avec sidecar: " << (avecSidecar ? "Oui" : "Non") << endl;
    }
};

// ============================================================================
// CLASSE DÉRIVÉE : Camion
// ============================================================================

class Camion : public Vehicle {
private:
    double chargeMaximale;
    int nombreEssieux;

public:
    Camion(const string& mat, const string& marq,
        const string& mod, const string& coul, int an,
        double charge, int essieux)
        : Vehicle(mat, marq, mod, coul, an), chargeMaximale(charge),
        nombreEssieux(essieux) {
    }

    double getChargeMaximale() const { return chargeMaximale; }
    int getNombreEssieux() const { return nombreEssieux; }

    string getType() const override { return "Camion"; }
    double getTarifHoraire() const override { return 20.0; }

    void afficher() const override {
        cout << *this << "\n"
            << "Charge maximale: " << chargeMaximale << " tonnes\n"
            << "Nombre d'essieux: " << nombreEssieux << endl;
    }
};

// ============================================================================
// HIÉRARCHIE D'EXCEPTIONS PERSONNALISÉES
// ============================================================================

class ParkingException : public exception {
protected:
    string message;

public:
    explicit ParkingException(const string& msg) : message(msg) {}

    virtual const char* what() const noexcept override {
        return message.c_str();
    }
};

class PlaceOccupeeException : public ParkingException {
public:
    explicit PlaceOccupeeException(int numeroPlace)
        : ParkingException("La place numero " + to_string(numeroPlace) +
            " est deja occupee!") {
    }
};

class PlaceInexistanteException : public ParkingException {
public:
    explicit PlaceInexistanteException(int numeroPlace)
        : ParkingException("La place numero " + to_string(numeroPlace) +
            " n'existe pas!") {
    }
};

class VehiculeNonTrouveException : public ParkingException {
public:
    explicit VehiculeNonTrouveException(const string& matricule)
        : ParkingException("Le vehicule avec matricule " + matricule +
            " n'a pas ete trouve!") {
    }
};

class ParkingCompletException : public ParkingException {
public:
    ParkingCompletException()
        : ParkingException("Le parking est complet! Aucune place disponible.") {
    }
};

class PlaceDejaLibreException : public ParkingException {
public:
    explicit PlaceDejaLibreException(int numeroPlace)
        : ParkingException("La place numero " + to_string(numeroPlace) +
            " est deja libre!") {
    }
};

// ============================================================================
// CLASSE : PlaceParking
// ============================================================================

class PlaceParking {
private:
    int numero;
    bool occupee;
    shared_ptr<Vehicle> vehicule;
    chrono::system_clock::time_point heureArrivee;

public:
    explicit PlaceParking(int num)
        : numero(num), occupee(false), vehicule(nullptr) {
    }

    int getNumero() const { return numero; }
    bool isOccupee() const { return occupee; }
    shared_ptr<Vehicle> getVehicule() const { return vehicule; }

    void garer(shared_ptr<Vehicle> v) {
        if (occupee) {
            throw PlaceOccupeeException(numero);
        }
        vehicule = v;
        occupee = true;
        heureArrivee = chrono::system_clock::now();
    }

    shared_ptr<Vehicle> liberer() {
        if (!occupee) {
            throw PlaceDejaLibreException(numero);
        }
        auto v = vehicule;
        vehicule = nullptr;
        occupee = false;
        return v;
    }

    double calculerMontant() const {
        if (!occupee || !vehicule) {
            return 0.0;
        }

        auto maintenant = chrono::system_clock::now();
        auto duree = chrono::duration_cast<chrono::hours>(
            maintenant - heureArrivee);

        int heures = duree.count() > 0 ? static_cast<int>(duree.count()) : 1;
        return heures * vehicule->getTarifHoraire();
    }

    friend ostream& operator<<(ostream& os, const PlaceParking& p) {
        os << "Place " << p.numero << ": ";
        if (p.occupee && p.vehicule) {
            os << "Occupee par " << p.vehicule->getType()
                << " (" << p.vehicule->getMatricule() << ")";
        }
        else {
            os << "Libre";
        }
        return os;
    }

    bool operator<(const PlaceParking& other) const {
        return numero < other.numero;
    }
};

// ============================================================================
// CLASSE : Parking (gestion globale)
// ============================================================================

class Parking {
private:
    string nom;
    int capaciteMax;
    vector<unique_ptr<PlaceParking>> places;
    map<string, int> vehiculesParques;

public:
    Parking(const string& n, int capacite)
        : nom(n), capaciteMax(capacite) {
        for (int i = 1; i <= capacite; ++i) {
            places.push_back(make_unique<PlaceParking>(i));
        }
    }

    string getNom() const { return nom; }
    int getCapacite() const { return capaciteMax; }

    int getNombrePlacesLibres() const {
        int count = 0;
        for (const auto& place : places) {
            if (!place->isOccupee()) {
                count++;
            }
        }
        return count;
    }

    int getNombrePlacesOccupees() const {
        return capaciteMax - getNombrePlacesLibres();
    }

    int trouverPlaceLibre() const {
        for (const auto& place : places) {
            if (!place->isOccupee()) {
                return place->getNumero();
            }
        }
        return -1;
    }

    int garerVehicule(shared_ptr<Vehicle> v) {
        if (vehiculesParques.find(v->getMatricule()) != vehiculesParques.end()) {
            throw ParkingException("Le vehicule " + v->getMatricule() +
                " est deja gare dans ce parking!");
        }

        int numeroPlace = trouverPlaceLibre();
        if (numeroPlace == -1) {
            throw ParkingCompletException();
        }

        places[numeroPlace - 1]->garer(v);
        vehiculesParques[v->getMatricule()] = numeroPlace;
        return numeroPlace;
    }

    int garerVehiculePlace(shared_ptr<Vehicle> v, int numeroPlace) {
        if (numeroPlace < 1 || numeroPlace > capaciteMax) {
            throw PlaceInexistanteException(numeroPlace);
        }

        if (vehiculesParques.find(v->getMatricule()) != vehiculesParques.end()) {
            throw ParkingException("Le vehicule " + v->getMatricule() +
                " est deja gare dans ce parking!");
        }

        places[numeroPlace - 1]->garer(v);
        vehiculesParques[v->getMatricule()] = numeroPlace;
        return numeroPlace;
    }

    double libererVehicule(const string& matricule) {
        auto it = vehiculesParques.find(matricule);
        if (it == vehiculesParques.end()) {
            throw VehiculeNonTrouveException(matricule);
        }

        int numeroPlace = it->second;
        double montant = places[numeroPlace - 1]->calculerMontant();
        places[numeroPlace - 1]->liberer();
        vehiculesParques.erase(it);
        return montant;
    }

    int trouverPlaceVehicule(const string& matricule) const {
        auto it = vehiculesParques.find(matricule);
        if (it != vehiculesParques.end()) {
            return it->second;
        }
        return -1;
    }

    shared_ptr<Vehicle> trouverVehicule(const string& matricule) const {
        int numeroPlace = trouverPlaceVehicule(matricule);
        if (numeroPlace != -1) {
            return places[numeroPlace - 1]->getVehicule();
        }
        return nullptr;
    }

    void afficherEtat() const {
        cout << "\n=== Etat du parking: " << nom << " ===" << endl;
        cout << "Capacite totale: " << capaciteMax << " places" << endl;
        cout << "Places occupees: " << getNombrePlacesOccupees() << endl;
        cout << "Places libres: " << getNombrePlacesLibres() << endl;
        cout << "\nDetail des places:" << endl;

        for (const auto& place : places) {
            cout << "  " << *place << endl;
        }
    }

    void afficherVehicules() const {
        cout << "\n=== Vehicules gares ===" << endl;
        if (vehiculesParques.empty()) {
            cout << "Aucun vehicule gare." << endl;
            return;
        }

        for (const auto& pair : vehiculesParques) {
            int numeroPlace = pair.second;
            auto vehicule = places[numeroPlace - 1]->getVehicule();
            cout << "\nPlace " << numeroPlace << ":" << endl;
            vehicule->afficher();
            cout << "Montant actuel: " << fixed << setprecision(2)
                << places[numeroPlace - 1]->calculerMontant() << " DH" << endl;
        }
    }

    void afficherStatistiques() const {
        cout << "\n=== Statistiques du parking ===" << endl;
        cout << "Nom: " << nom << endl;
        cout << "Capacite totale: " << capaciteMax << endl;
        cout << "Places occupees: " << getNombrePlacesOccupees()
            << " (" << (getNombrePlacesOccupees() * 100.0 / capaciteMax)
            << "%)" << endl;
        cout << "Places libres: " << getNombrePlacesLibres()
            << " (" << (getNombrePlacesLibres() * 100.0 / capaciteMax)
            << "%)" << endl;

        map<string, int> typesCount;
        for (const auto& pair : vehiculesParques) {
            auto vehicule = places[pair.second - 1]->getVehicule();
            typesCount[vehicule->getType()]++;
        }

        cout << "\nRepartition par type de vehicule:" << endl;
        for (const auto& pair : typesCount) {
            cout << "  " << pair.first << ": " << pair.second << endl;
        }
    }

    PlaceParking& operator[](int index) {
        if (index < 0 || index >= capaciteMax) {
            throw PlaceInexistanteException(index + 1);
        }
        return *places[index];
    }

    friend ostream& operator<<(ostream& os, const Parking& p) {
        os << "Parking " << p.nom << " - "
            << p.getNombrePlacesOccupees() << "/" << p.capaciteMax << " occupees";
        return os;
    }

    Parking& operator+=(shared_ptr<Vehicle> v) {
        garerVehicule(v);
        return *this;
    }
};

// ============================================================================
// FONCTIONS UTILITAIRES
// ============================================================================

void configureConsole() {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
}

void afficherMenu() {
    cout << "\n========================================" << endl;
    cout << "   SYSTEME DE GESTION DE PARKING" << endl;
    cout << "========================================" << endl;
    cout << "1. Garer un vehicule (automatique)" << endl;
    cout << "2. Garer un vehicule (place specifique)" << endl;
    cout << "3. Liberer un vehicule" << endl;
    cout << "4. Afficher l'etat du parking" << endl;
    cout << "5. Afficher tous les vehicules" << endl;
    cout << "6. Rechercher un vehicule" << endl;
    cout << "7. Afficher les statistiques" << endl;
    cout << "8. Test automatique complet" << endl;
    cout << "0. Quitter" << endl;
    cout << "========================================" << endl;
    cout << "Votre choix: ";
}

void testAutomatique(Parking& parking) {
    cout << "\n***** TEST AUTOMATIQUE COMPLET *****\n" << endl;

    try {
        cout << "=== Test 1: Creation de vehicules ===" << endl;
        auto voiture1 = make_shared<Voiture>("12345-A-67", "Renault", "Clio",
            "Rouge", 2020, 5, "Essence");
        auto voiture2 = make_shared<Voiture>("67890-B-12", "Peugeot", "208",
            "Bleu", 2021, 3, "Diesel");
        auto moto1 = make_shared<Moto>("54321-M-34", "Yamaha", "MT-07",
            "Noir", 2019, 689);
        auto moto2 = make_shared<Moto>("98765-M-56", "Honda", "CBR",
            "Rouge", 2022, 1000);
        auto camion1 = make_shared<Camion>("11111-C-89", "Mercedes", "Actros",
            "Blanc", 2018, 25.5, 3);
        cout << "OK - 5 vehicules crees avec succes" << endl;

        cout << "\n=== Test 2: Polymorphisme (affichage) ===" << endl;
        vector<shared_ptr<Vehicle>> vehicules = { voiture1, moto1, camion1 };
        for (const auto& v : vehicules) {
            v->afficher();
            cout << "---" << endl;
        }

        cout << "\n=== Test 3: Garage automatique ===" << endl;
        int place1 = parking.garerVehicule(voiture1);
        cout << "OK - Voiture garee a la place " << place1 << endl;
        int place2 = parking.garerVehicule(moto1);
        cout << "OK - Moto garee a la place " << place2 << endl;
        int place3 = parking.garerVehicule(camion1);
        cout << "OK - Camion gare a la place " << place3 << endl;

        cout << "\n=== Test 4: Garage avec place specifique ===" << endl;
        int place4 = parking.garerVehiculePlace(voiture2, 5);
        cout << "OK - Voiture garee a la place " << place4 << endl;

        cout << "\n=== Test 5: Surcharge operateur += ===" << endl;
        parking += moto2;
        cout << "OK - Moto garee avec operateur +=" << endl;

        parking.afficherEtat();

        cout << "\n=== Test 6: Recherche de vehicule ===" << endl;
        string matriculeRecherche = "12345-A-67";
        int placeRecherche = parking.trouverPlaceVehicule(matriculeRecherche);
        if (placeRecherche != -1) {
            cout << "OK - Vehicule " << matriculeRecherche
                << " trouve a la place " << placeRecherche << endl;
        }

        cout << "\n=== Test 7: Surcharge operateur [] ===" << endl;
        cout << "Place [0]: " << parking[0] << endl;
        cout << "Place [1]: " << parking[1] << endl;

        cout << "\n=== Test 8: Liberation de vehicule ===" << endl;
        double montant = parking.libererVehicule("54321-M-34");
        cout << "OK - Moto liberee. Montant a payer: " << montant << " DH" << endl;

        cout << "\n=== Test 9: Test d'exceptions ===" << endl;
        cout << "Test: Tentative de garer sur une place occupee..." << endl;
        try {
            parking.garerVehiculePlace(moto1, 1);
        }
        catch (const PlaceOccupeeException& e) {
            cout << "OK - Exception capturee: " << e.what() << endl;
        }

        cout << "\nTest: Tentative d'acces a une place inexistante..." << endl;
        try {
            parking.garerVehiculePlace(moto1, 999);
        }
        catch (const PlaceInexistanteException& e) {
            cout << "OK - Exception capturee: " << e.what() << endl;
        }

        cout << "\nTest: Tentative de liberer un vehicule inexistant..." << endl;
        try {
            parking.libererVehicule("XXXXX-X-XX");
        }
        catch (const VehiculeNonTrouveException& e) {
            cout << "OK - Exception capturee: " << e.what() << endl;
        }

        cout << "\nTest: Remplissage du parking..." << endl;
        try {
            for (int i = 0; i < 20; i++) {
                auto v = make_shared<Voiture>("TEST-" + to_string(i), "Test",
                    "Model", "Couleur", 2020, 4, "Essence");
                parking += v;
            }
        }
        catch (const ParkingCompletException& e) {
            cout << "OK - Exception capturee: " << e.what() << endl;
        }

        parking.afficherStatistiques();

        cout << "\n=== Test 10: Surcharge operateur == ===" << endl;
        auto v1 = make_shared<Voiture>("TEST123", "Marque", "Model",
            "Couleur", 2020, 4, "Essence");
        auto v2 = make_shared<Voiture>("TEST123", "Autre", "Autre",
            "Autre", 2021, 5, "Diesel");
        auto v3 = make_shared<Voiture>("TEST456", "Marque", "Model",
            "Couleur", 2020, 4, "Essence");

        if (*v1 == *v2) {
            cout << "OK - v1 == v2 (meme matricule)" << endl;
        }
        if (!(*v1 == *v3)) {
            cout << "OK - v1 != v3 (matricules differents)" << endl;
        }

        cout << "\n***** FIN DU TEST AUTOMATIQUE *****" << endl;

    }
    catch (const exception& e) {
        cout << "Erreur inattendue: " << e.what() << endl;
    }
}

void saisirVehicule(Parking& parking) {
    int type;
    cout << "\nType de vehicule:" << endl;
    cout << "1. Voiture" << endl;
    cout << "2. Moto" << endl;
    cout << "3. Camion" << endl;
    cout << "Choix: ";
    cin >> type;
    cin.ignore();

    string matricule, marque, modele, couleur;
    int annee;

    cout << "Matricule: ";
    getline(cin, matricule);
    cout << "Marque: ";
    getline(cin, marque);
    cout << "Modele: ";
    getline(cin, modele);
    cout << "Couleur: ";
    getline(cin, couleur);
    cout << "Annee: ";
    cin >> annee;
    cin.ignore();

    try {
        shared_ptr<Vehicle> vehicule;

        if (type == 1) {
            int portes;
            string carburant;
            cout << "Nombre de portes: ";
            cin >> portes;
            cin.ignore();
            cout << "Type de carburant: ";
            getline(cin, carburant);
            vehicule = make_shared<Voiture>(matricule, marque, modele,
                couleur, annee, portes, carburant);
        }
        else if (type == 2) {
            int cylindree;
            cout << "Cylindree (cc): ";
            cin >> cylindree;
            vehicule = make_shared<Moto>(matricule, marque, modele,
                couleur, annee, cylindree);
        }
        else if (type == 3) {
            double charge;
            int essieux;
            cout << "Charge maximale (tonnes): ";
            cin >> charge;
            cout << "Nombre d'essieux: ";
            cin >> essieux;
            vehicule = make_shared<Camion>(matricule, marque, modele,
                couleur, annee, charge, essieux);
        }
        else {
            cout << "Type invalide!" << endl;
            return;
        }

        int numeroPlace = parking.garerVehicule(vehicule);
        cout << "OK - Vehicule gare avec succes a la place " << numeroPlace << endl;

    }
    catch (const ParkingException& e) {
        cout << "Erreur: " << e.what() << endl;
    }
}

// ============================================================================
// FONCTION PRINCIPALE
// ============================================================================

int main() {
    configureConsole();

    Parking parking("Centre Ville", 10);

    cout << "========================================" << endl;
    cout << "  BIENVENUE AU PARKING CENTRE VILLE" << endl;
    cout << "========================================" << endl;
    cout << parking << endl;

    int choix;
    do {
        afficherMenu();
        cin >> choix;
        cin.ignore();

        try {
            switch (choix) {
            case 1:
                saisirVehicule(parking);
                break;

            case 2: {
                int numPlace;
                cout << "Numero de place souhaite: ";
                cin >> numPlace;
                cin.ignore();
                // Simplifié - réutilise saisirVehicule
                cout << "Fonctionnalite disponible via option 1" << endl;
                break;
            }

            case 3: {
                string matricule;
                cout << "Matricule du vehicule a liberer: ";
                getline(cin, matricule);
                double montant = parking.libererVehicule(matricule);
                cout << "OK - Vehicule libere. Montant a payer: "
                    << fixed << setprecision(2) << montant << " DH" << endl;
                break;
            }

            case 4:
                parking.afficherEtat();
                break;

            case 5:
                parking.afficherVehicules();
                break;

            case 6: {
                string matricule;
                cout << "Matricule a rechercher: ";
                getline(cin, matricule);
                int place = parking.trouverPlaceVehicule(matricule);
                if (place != -1) {
                    cout << "OK - Vehicule trouve a la place " << place << endl;
                    auto vehicule = parking.trouverVehicule(matricule);
                    vehicule->afficher();
                }
                else {
                    cout << "ERREUR - Vehicule non trouve" << endl;
                }
                break;
            }

            case 7:
                parking.afficherStatistiques();
                break;

            case 8:
                testAutomatique(parking);
                break;

            case 0:
                cout << "\n========================================" << endl;
                cout << "  Merci d'avoir utilise notre parking!" << endl;
                cout << "========================================" << endl;
                break;

            default:
                cout << "Choix invalide! Veuillez reessayer." << endl;
            }

        }
        catch (const ParkingException& e) {
            cout << "Erreur: " << e.what() << endl;
        }
        catch (const exception& e) {
            cout << "Erreur inattendue: " << e.what() << endl;
        }

    } while (choix != 0);

    cout << "\nAppuyez sur Entree pour quitter...";
    cin.get();

    return 0;
}