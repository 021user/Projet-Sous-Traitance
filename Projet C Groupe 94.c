#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#pragma warning (disable : 4996)


enum { MAX_CARACTERE = 31, MAX = 50, MAX_MISSION = 500, CODE_RETOUR = 38, DEFAUT = -1, SOUSTRAITANCE = 5, ROLE_SAUV = 3}; // Aucune mission ne peut �tre egal � -1
typedef struct {
	char info_retour[CODE_RETOUR];
}Resultat;


typedef struct {
	char role[ROLE_SAUV], nom[MAX_CARACTERE];
}Entreprise;


typedef struct {
	char dispo[MAX_CARACTERE];  // Initialiser � Non-attribu�
	char nom[MAX_CARACTERE];
	int id_entreprise, niveau;
	int accepte;
	float remuneration;

	int code_retour, id_anterieur; // id_anterieur nous permet de conna�tre l'identifiant de l'ancienne mission o� l'on a fait un rapport
}Mission;

/**
 * @brief Publie une nouvelle mission
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] inscription Tableau contenant les informations des entreprises inscrites
 * @param[in,out] mission Tableau contenant les informations des missions
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 * @param[in] nb_mission Nombre de missions publi�es actuellement
 */
void Publication_mission(char* saisie, const Entreprise inscription[], Mission mission[], const int* nb_inscription, int* nb_mission) {
	int id = 0;
	float remuneration = 0.f;
	if (*nb_mission >= MAX_MISSION) {
		printf("Plus aucune mission ne peut etre publiee\n");
		return;
	}
	scanf("%s", saisie);
	id = atoi(saisie); // Permet de transformer une cha�ne de caract�re en entier
	if ((*nb_inscription == 0) || (id > *nb_inscription) || (id < 0) || strcmp(inscription[id - 1].role, "OP") != 0) {
		scanf("%s", saisie);
		scanf("%s", saisie);
		printf("Identifiant incorrect\n");
		return;
	}
	mission[*nb_mission].id_entreprise = id;
	scanf("%s", saisie);
	strcpy(mission[*nb_mission].nom, saisie);
	scanf("%s", saisie);
	remuneration = atof(saisie);
	if (remuneration <= 0) {
		printf("Remuneration incorrecte\n");
		return;
	}
	mission[*nb_mission].remuneration = remuneration;
	printf("Mission publiee (%d)\n", *nb_mission + 1);
	strcpy(mission[*nb_mission].dispo, "Non-attribuee");
	mission[*nb_mission].niveau = 0;
	mission[*nb_mission].id_anterieur = DEFAUT;
	mission[*nb_mission].code_retour = DEFAUT;
	*nb_mission += 1;
}


/**
 * @brief Permet � une entreprise de s'inscrire
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] nb_inscription Nombre d'entreprises d�j� inscrites
 * @param[in,out] inscription Tableau contenant les informations des entreprises inscrites
 */
void Inscription(char* saisie, int* nb_inscription, Entreprise inscription[]) {
	scanf("%s", saisie);
	if (*nb_inscription >= MAX) {
		printf("Plus aucune Entreprise ne peut s'inscrire\n");
		return;
	}
	// V�rifie si le r�le est valide, sinon affiche une erreur
	if ((strcmp(saisie, "OP") == NULL) || (strcmp(saisie, "AG") == NULL) || (strcmp(saisie, "IN") == NULL)) {
		strcpy(inscription[*nb_inscription].role, saisie);
	}
	else {
		// �vite de vider scanf si le nom est d�j� entr�
		scanf("%s", saisie);
		printf("Role incorrect\n");
		return;
	}
	// R�cup�re et v�rifie si le nom de l'entreprise ne depasse pas la taille maximum
	scanf("%30s", saisie);
	if (strlen(saisie) > MAX_CARACTERE) {
		printf("Nom incorrect\n");
		return;
	}
	// V�rifie si les noms sont identiques, sinon affiche une erreur
	if (*nb_inscription > 0) {
		for (int verif = 0; verif < *nb_inscription; ++verif) {
			if (strcmp(saisie, inscription[verif].nom) == NULL) {
				printf("Nom incorrect\n");
				return;
			}
		}
	}
	// Enregistre le nom, l'identifiant et confirme l'inscription
	strcpy(inscription[*nb_inscription].nom, saisie);
	printf("Inscription realisee (%d)\n", *nb_inscription + 1);
	*nb_inscription += 1;
}


/**
 * @brief Affiche les missions non attribu�es avec les d�tails de l'entreprise correspondante
 *
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 * @param[in] nb_mission Nombre de missions disponibles
 * @param[in] inscription Tableau contenant les informations des entreprises inscrites
 * @param[in] mission Tableau contenant les informations des missions
 */
void Consultation(const int* nb_inscription, const int* nb_mission, const Entreprise inscription[], const Mission mission[]) {
	char nom[MAX_CARACTERE]; // D�claration d'une variable pour stocker le nom de l'entreprise

	if (*nb_mission == 0) { // V�rifie s'il n'y a aucune mission
		printf("Aucune mission disponible\n");
	}
	else {
		int condition = 1; // Permet de savoir si l'on est rentr� dans la fonction
		for (int i = 0; i < *nb_mission; ++i) {
			// V�rifie si la mission est non attribu�e et l'affiche si c'est le cas
			if (strcmp(mission[i].dispo, "Non-attribuee") == NULL) {
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom); // R�cup�re le nom de l'entreprise
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
				condition = 0; // Indique qu'au moins une mission a �t� trouv�e
			}
		}
		// Si on n'est pas entr� dans la fonction, afficher ce message
		if (condition == 1) {
			printf("Aucune mission disponible\n");
		}
	}
}


/**
 * @brief Attribue une mission � une entreprise donn�e
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 * @param[in] nb_mission Nombre de missions disponibles
 * @param[in,out] inscription Tableau contenant les informations des entreprises inscrites
 * @param[in,out] mission Tableau contenant les informations des missions
 */
void Attribution(char* saisie, const int* nb_inscription, const int* nb_mission, Entreprise inscription[], Mission mission[]) {
	int copy = 0;
	int test = 0;

	scanf("%s", saisie);
	copy = atoi(saisie); // Convertit l'entr�e en entier

	// V�rifie la validit� de l'entreprise saisie pour attribution de mission
	if ((*nb_inscription < copy) || (copy <= 0) || (strcmp(inscription[copy - 1].role, "OP") == NULL)) {
		scanf("%s", saisie);
		printf("Entreprise incorrecte\n");
		return;
	}

	scanf("%s", saisie);
	test = atoi(saisie); // Convertit l'entr�e en entier

	// V�rifie la validit� de la mission saisie pour attribution
	if ((*nb_mission < test) || (test <= 0) || (strcmp(mission[test - 1].dispo, "Attribuee") == NULL)
		|| (strcmp(mission[test - 1].dispo, "Terminee") == NULL)) {
		printf("Mission incorrecte\n");
		return;
	}

	// V�rifie que l'entreprise n'a pas d�j� �t� attribu�e � cette mission
	if (mission[test - 1].id_anterieur != DEFAUT) {
		while (mission[test - 1].id_anterieur != DEFAUT) {
			if (mission[test - 1].accepte == copy) {
				printf("Entreprise incorrecte\n");
				return;
			}
			test = mission[test - 1].id_anterieur;
		}
		if (mission[test - 1].accepte == copy) {
			printf("Entreprise incorrecte\n");
			return;
		}
	}

	// Attribution de la mission � l'entreprise donn�e
	strcpy(mission[atoi(saisie) - 1].dispo, "Attribuee");
	mission[atoi(saisie) - 1].accepte = copy;
	printf("Acceptation enregistree\n");
}


/**
 * @brief G�n�re un rapport pour une mission termin�e et effectue les actions correspondantes
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in,out] mission Tableau contenant les informations des missions
 * @param[in,out] nb_mission Nombre de missions disponibles
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 */
void Rapport_mission(char* saisie, Mission mission[], int* nb_mission, const int* nb_inscription) {
	int id = 0;
	scanf("%s", saisie);
	id = atoi(saisie); // Convertit l'entr�e en entier (id de mission)

	// V�rifie la validit� de l'ID de mission et son �tat pour rapport
	if ((strcmp(mission[id - 1].dispo, "Non-attribuee") == NULL) || *nb_mission == 0 ||
		(strcmp(mission[id - 1].dispo, "Terminee") == NULL || id > *nb_mission || id <= 0)) {
		scanf("%s", saisie);
		printf("Mission incorrecte\n");
		return;
	}

	scanf("%s", saisie);
	int code_retour = 0;
	code_retour = atoi(saisie); // Convertit l'entr�e en entier (code de retour)

	float majoration = 0.0f;
	if (code_retour < 0 || code_retour > 3) {
		printf("Code de retour incorrect\n");
		return;
	}
	switch (code_retour) {
	case 2:
		majoration = 5.5;
		break;
	case 3:
		majoration = 4.0;
		break;
	default:
		break;
	}

	int copie_id = id;

	// Termine la mission actuelle et ses missions ant�rieures si elles existent
	if (mission[copie_id - 1].niveau != 0 && code_retour == 0) {
		while (mission[copie_id - 1].niveau != 0) {
			strcpy(mission[copie_id - 1].dispo, "Terminee");
			copie_id = mission[copie_id - 1].id_anterieur;
		}
		strcpy(mission[copie_id - 1].dispo, "Terminee");
	}

	mission[id - 1].code_retour = code_retour; // Enregistre le code de retour
	strcpy(mission[id - 1].dispo, "Terminee"); // Marque la mission comme termin�e

	if (code_retour != 0) {
		// Cr�e une nouvelle mission non-attribu�e pour l'entreprise
		mission[*nb_mission].id_entreprise = mission[id - 1].id_entreprise;
		strcpy(mission[*nb_mission].nom, mission[id - 1].nom);
		mission[*nb_mission].remuneration = mission[id - 1].remuneration * (1 + majoration / 100.0);
		strcpy(mission[*nb_mission].dispo, "Non-attribuee");
		mission[*nb_mission].niveau = 0;

		// Partie Rapport
		mission[*nb_mission].code_retour = code_retour;
		mission[*nb_mission].id_anterieur = id;
		*nb_mission += 1;
		printf("Rapport enregistre (%d)\n", *nb_mission);
	}
	else {
		printf("Rapport enregistre\n");
	}
}


/**
 * @brief Affiche les d�tails d'une mission sp�cifique avec ses r�sultats et ses missions ant�rieures
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 * @param[in] nb_mission Nombre de missions disponibles
 * @param[in] inscription Tableau contenant les informations des entreprises inscrites
 * @param[in] mission Tableau contenant les informations des missions
 * @param[in] resultat Tableau contenant les r�sultats des missions
 */
void Detail(char* saisie, const int* nb_inscription, const int* nb_mission, const Entreprise inscription[], const Mission mission[], const Resultat resultat[]) {
	scanf("%s", saisie);
	int id_ = atoi(saisie); // Convertit l'entr�e en entier (id_ de mission)

	// V�rifie la validit� de l'ID de mission
	if ((id_ > *nb_mission) || id_ <= 0) {
		printf("Identifiant incorrect\n");
		return;
	}

	char nom[MAX_CARACTERE];
	strcpy(nom, inscription[mission[id_ - 1].id_entreprise - 1].nom);
	printf("%d  %s  %s  %.2f (%d)\n", id_, mission[id_ - 1].nom, nom, mission[id_ - 1].remuneration, mission[id_ - 1].niveau);

	if (mission[id_ - 1].id_anterieur == DEFAUT && mission[id_ - 1].code_retour != DEFAUT) {
		printf("%s\n", resultat[mission[id_ - 1].code_retour].info_retour);
	}
	else if (mission[id_ - 1].id_anterieur == DEFAUT) {
		return;
	}
	else {
		int sauvegarde[ROLE_SAUV]; // Valeur DEFAUT permet de cr�er un tableau sans nombre magique
		int indexe = 0; // Permet de modifier les valeurs de sauvegarde

		// Parcourt les missions ant�rieures li�es � cette mission pour afficher les anciens code retour
		// Verifie qu'on n'affiche qu'une seule fois chaque message retour
		while (mission[id_ - 1].id_anterieur != DEFAUT) {
			if (mission[id_ - 1].code_retour == 0) {
				printf("%s\n", resultat[mission[id_ - 1].code_retour].info_retour);
				return;
			}
			else if (mission[id_ - 1].code_retour == sauvegarde[0] || mission[id_ - 1].code_retour == sauvegarde[1] ||
				mission[id_ - 1].code_retour == sauvegarde[2]) {
				id_ = mission[id_ - 1].id_anterieur;
			}
			else {
				printf("%s\n", resultat[mission[id_ - 1].code_retour].info_retour);
				sauvegarde[indexe] = mission[id_ - 1].code_retour;
				indexe += 1;
				id_ = mission[id_ - 1].id_anterieur;
			}
		}

		// V�rifie les retours sauvegard�s pour �viter les doublons
		if (mission[id_ - 1].code_retour != sauvegarde[0] && mission[id_ - 1].code_retour != sauvegarde[1] &&
			mission[id_ - 1].code_retour != sauvegarde[2]) {
			if (*nb_mission > 2) {
				printf("%s\n", resultat[mission[id_ - 1].code_retour].info_retour);
			}
		}
	}
}


/**
 * @brief Affiche un r�capitulatif des missions li�es � une entreprise sp�cifique
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] mission Tableau contenant les informations des missions
 * @param[in] inscription Tableau contenant les informations des entreprises inscrites
 * @param[in] nb_mission Nombre de missions disponibles
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 */
void recapitulatif(char* saisie, const Mission mission[], const Entreprise inscription[], const int* nb_mission, const int* nb_inscription) {
	scanf("%s", saisie);
	int id_e = atoi(saisie);
	int boolean = 0;
	char nom[MAX_CARACTERE];
	if (id_e > *nb_inscription || id_e <= 0) {
		printf("Entreprise incorrecte\n");
		return;
	}
	if (strcmp(inscription[id_e - 1].role, "IN") != NULL) {
		for (int i = 0; i < *nb_mission; ++i) {
			if (mission[i].id_entreprise == id_e && strcmp(mission[i].dispo, "Non-attribuee") == NULL) {
				if (boolean == 0) {
					boolean = 1;
					printf("* non attribuees\n");
				}
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom);
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
			}
		}
		boolean = 0;
		for (int i = 0; i < *nb_mission; ++i) {
			if (mission[i].id_entreprise == id_e && (strcmp(mission[i].dispo, "Attribuee") == NULL)) {
				if (boolean == 0) {
					boolean = 1;
					printf("* attribuees\n");
				}
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom);
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
			}
		}
		boolean = 0;
		for (int i = 0; i < *nb_mission; ++i) {
			if (mission[i].id_entreprise == id_e && strcmp(mission[i].dispo, "Terminee") == NULL) {
				if (boolean == 0) {
					boolean = 1;
					printf("* terminees\n");
				}
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom);
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
			}
		}
	}
	if (strcmp(inscription[id_e - 1].role, "OP") != NULL) {
		for (int i = 0; i < *nb_mission; ++i) {
			if (mission[i].accepte == id_e && strcmp(mission[i].dispo, "Attribuee") == NULL) {
				if (boolean == 0) {
					boolean = 1;
					printf("* a realiser\n");
				}
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom);
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
			}
		}
		boolean = 0;
		for (int i = 0; i < *nb_mission; ++i) {
			if (mission[i].accepte == id_e && strcmp(mission[i].dispo, "Terminee") == NULL) {
				if (boolean == 0) {
					boolean = 1;
					printf("* realisees\n");
				}
				strcpy(nom, inscription[mission[i].id_entreprise - 1].nom);
				printf("%d  %s  %s  %.2f (%d)\n", i + 1, mission[i].nom, nom, mission[i].remuneration, mission[i].niveau);
			}
		}
	}
}


/**
 * @brief Effectue la sous-traitance d'une mission � une entreprise de sous-traitance
 *
 * @param[in] saisie Cha�ne de caract�res utilis�e pour la saisie des donn�es
 * @param[in] nb_inscription Nombre d'entreprises inscrites
 * @param[in,out] nb_mission Nombre de missions disponibles
 * @param[in,out] mission Tableau contenant les informations des missions
 * @param[in] inscription Tableau contenant les informations des entreprises inscrites
 */
void sous_traitance(char* saisie, const int* nb_inscription, int* nb_mission, Mission mission[], const Entreprise inscription[]) {
	scanf("%s", saisie);
	int entreprise_id = atoi(saisie); // Convertit l'entr�e en ID d'entreprise

	// V�rifie la validit� de l'ID d'entreprise
	if (*nb_inscription == 0 || entreprise_id > *nb_inscription || entreprise_id < 0 || strcmp(inscription[entreprise_id - 1].role, "AG") != NULL) {
		scanf("%s", saisie);
		scanf("%s", saisie);
		printf("Entreprise incorrecte\n");
		return;
	}

	scanf("%s", saisie);
	int mission_id = atoi(saisie); // Convertit l'entr�e en ID de mission

	// V�rifie la validit� de l'ID de mission
	if ((*nb_mission < mission_id) || mission_id <= 0 || strcmp(mission[mission_id - 1].dispo, "Attribuee") == NULL ||
		strcmp(mission[mission_id - 1].dispo, "Terminee") == NULL || mission[mission_id - 1].niveau == SOUSTRAITANCE) {
		printf("Mission incorrecte\n");
		return;
	}

	scanf("%s", saisie);
	float mission_remu = 0.f;
	mission_remu = atof(saisie); // Convertit l'entr�e en r�mun�ration de mission

	// V�rifie la validit� de la r�mun�ration
	if (mission_remu <= 0) {
		printf("Remuneration incorrecte\n");
		return;
	}

	// Attribution de la mission � une entreprise de sous-traitance
	strcpy(mission[mission_id - 1].dispo, "Attribuee");

	// Cr�ation d'une nouvelle mission non-attribu�e pour l'entreprise de sous-traitance
	mission[*nb_mission].id_entreprise = entreprise_id;
	strcpy(mission[*nb_mission].nom, mission[mission_id - 1].nom);
	mission[*nb_mission].remuneration = mission_remu;
	strcpy(mission[*nb_mission].dispo, "Non-attribuee");
	mission[*nb_mission].niveau = mission[mission_id - 1].niveau + 1;
	mission[*nb_mission].id_anterieur = mission_id;
	mission[*nb_mission].code_retour = mission[mission_id - 1].code_retour;
	*nb_mission += 1;

	printf("Sous-traitance enregistree (%d)\n", *nb_mission);
}


int main() {
	char saisie[MAX_CARACTERE];
	int nb_inscription = 0, nb_mission = 0;
	Entreprise inscription[MAX];
	Mission mission[MAX_MISSION];
	const Resultat resultat[] = { {"Succes"},
		{"Local non accessible"},
		{"Pas de signal dans le boitier general"},
		{"Recepteur defectueux"} };
	do {
		scanf("%s", saisie);
		if (strcmp(saisie, "inscription") == NULL) {
			Inscription(saisie, &nb_inscription, inscription);
		}
		else if (strcmp(saisie, "mission") == NULL) {
			Publication_mission(saisie, inscription, mission, &nb_inscription, &nb_mission);
		}
		else if (strcmp(saisie, "consultation") == NULL) {
			Consultation(&nb_inscription, &nb_mission, inscription, mission);
		}
		else if (strcmp(saisie, "detail") == NULL) {
			Detail(saisie, &nb_inscription, &nb_mission, inscription, mission, resultat);
		}
		else if (strcmp(saisie, "acceptation") == NULL) {
			Attribution(saisie, &nb_inscription, &nb_mission, inscription, mission);
		}
		else if (strcmp(saisie, "rapport") == NULL) {
			Rapport_mission(saisie, mission, &nb_mission, &nb_inscription);
		}
		else if (strcmp(saisie, "recapitulatif") == NULL) {
			recapitulatif(saisie, mission, inscription, &nb_mission, &nb_inscription);
		}
		else if (strcmp(saisie, "sous-traitance") == NULL) {
			sous_traitance(saisie, &nb_inscription, &nb_mission, mission, inscription);
		}
	} while (strcmp(saisie, "exit") != NULL);
}