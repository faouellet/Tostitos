TOSTITOS: 

Transférer l'ownership a un organisation

Passer include-what-you-use dessus Tostitos
Les inclusions de .lib sont du spaghetti

CI:  
- passer plus de sanitizers sur le build Clang

IMPORTANT: Require return statement / main doit avoir return Void
IMPORTANT: Empêcher d'avoir fn/var avec même nom que des keywords

Revoir le format d'un fichier Chip16
cleanup d'enums dans Chip16
tests pour jumps
ajouter exemples dans la doc doxygen des AST nodes
introduire notion de priorité d'opérations
commentaires pour détailler le parsing (surtout ce qui est attendu à la fin des fonctions)
Refactoring des passes sémantiques (trop de répétitions)
Commenter ASTReader (mCurrentLine)
Trie for symbol table??
Tests pour Overload Resolution
Tests pour CallStack
Tests pour array function parameter et return type
Revoir les inclusions (surtout à la bordure TosLang/Tostitos)
friend accesseur ChildrenNodes pour ASTVisitor

Tests:
- binop entre identifiers
- binop avec call + kkchose
- variable decl dans une fonction (correct + errors)
- mise à jour du test de ASTVisitor
- Ajout commentaires dans les tests
- EOF dans déclaration de fonction

Plus tard, faire tests de sytème:
	- tri rapide
	- multiplication de matrices
	- somme des éléments d'un vecteur

Faire un fuzzer pour TosLang ??

Faire un Learn X in Y minutes