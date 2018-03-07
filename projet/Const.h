
#ifndef __ALL_H__
#define __ALL_H__

#define OUI 0

const int TRAVAILLEUR_FINI = -198;
const int TRAVAILEUR_DONNER_RESULTAT = -199;
const int TRAVAILEUR_TRANSACTION_FINI = -200;

typedef enum ETATS { RIEN=0, NOUVELLE, VIVANTE, MORTE};

#endif