#include <assert.h>
#include <math.h>

const double PI = 3.14159265359;
const double MAX_DECAY = 0.8;

double lr_fixed(double lr) { return lr; }

double lr_poly(double lr, double p, int iter, int total_steps) {
  assert(iter <= total_steps);
  // we can decay up to 80% of the LR
  double min_lr = lr * (1 - MAX_DECAY);
  return (lr - min_lr) * pow((1 - ((double)iter / (double)total_steps)), p) +
         min_lr;
}

double lr_cosine(double lr, int iter, int total_steps) {
  assert(iter <= total_steps);
  // we can decay up to 80% of the LR
  double min_lr = lr * (1 - MAX_DECAY);
  return (min_lr + 0.5 * (lr - min_lr) *
                       (1 + cos((double)iter / (double)total_steps * PI)));
}
