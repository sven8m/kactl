/**
 * Author: nweeks
 * Date: 2014-04-04
 * License: CC0
 * Source: cppref
 * Description: randint(0, n) returns random integer between 0 and n (included)
 * Time: Constant
 * Status: Tested
 */

#pragma once

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int randint(int lb, int ub) {
  return uniform_int_distribution<int>(lb, ub)(rng);
}
