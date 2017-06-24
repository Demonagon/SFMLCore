#ifndef GRADIANT__H
#define GRADIANT__H

/*
* Classe qui calcule des gradiants. Toutes les valeurs en entrées et sorties
* sont prises entre 0 et 1.
*/
class Gradiant
{
	public :
		static double cosGradiant(double start, double end, double value);
		static double polGradiant(double start, double end, double value, double power);
};

#endif
