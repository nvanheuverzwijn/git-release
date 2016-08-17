#ifndef ERRORS_H
#define ERRORS_H

#define E_TAGNOTFOUND 1 /* No tag could be found */
#define E_SEMVERSNOMATCH 2 /* Semvers could not match */
#define E_BRANCHNOTFOUND 3 /* Branch could not be found */
#define E_REMOTENOTFOUND 4 /* Remote (origin, upstream, etc) could not be found */
#define E_REMOTENOTFETCH 5 /* Remote (origin, upstream, etc) could not be fetched */

void die(const char* message);

#endif
