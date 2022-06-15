/* Test de l'objet UIKObj_ARexxSim
 * 1 - Faire "run trexx"
 * 2 - Executer ce script ARexx "rx tr.rexx"
 */

Options Results

Address TREXX.1

Abruti
if rc > 0 then say "rc="RC", erreur="TREXX.LASTERROR", "TREXX.LASTERRORTEXT
else say "rc = "RC", message resultat = "RESULT

Bon programme
if rc > 0 then say "rc="RC", erreur="TREXX.LASTERROR", "TREXX.LASTERRORTEXT
else say "rc = "RC", message resultat = "RESULT

Quitte
if rc > 0 then say "rc="RC", erreur="TREXX.LASTERROR", "TREXX.LASTERRORTEXT
else say "rc = "RC", message resultat = "RESULT
