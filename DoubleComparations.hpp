//
// Created by gdhsnlvr on 22.02.16.
//

#ifndef STONES_DOUBLECOMPARATIONS_HPP
#define STONES_DOUBLECOMPARATIONS_HPP

#define EPSILON (1e-9)
#define EQUALS(a, b) (abs(a - b) < EPSILON)
#define NOTEQUALS(a, b) (abs(a - b) >= EPSILON)
#define BETWEEN(x, l, r) (x > l - EPSILON && x < r + EPSILON)

#endif //STONES_DOUBLECOMPARATIONS_HPP
