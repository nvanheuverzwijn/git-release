#ifndef ERRORS_H
#define ERRORS_H

#define E_TAGNOTFOUND 1 /* No tag could be found */
#define E_SEMVERSNOMATCH 2 /* Semvers could not match */
#define E_BRANCHNOTFOUND 3 /* Branch could not be found */

void die(const char* message);

#endif
