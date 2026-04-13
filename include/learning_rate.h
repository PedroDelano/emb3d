#ifndef LEARNING_RATE_H
#define LEARNING_RATE_H

double lr_fixed(double lr);
double lr_poly(double lr, double p, int iter, int total_steps);
double lr_cosine(double lr, int iter, int total_steps);

#endif
