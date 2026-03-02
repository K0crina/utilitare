📁 Proiect: Utilitare Terminal 

📌 Descriere generală
Proiectul constă în dezvoltarea a patru utilitare de tip linie de comandă, care emulează comportamentul unor comenzi clasice din Linux (`ls`, `diff`, `cd`). 
Accentul cade pe procesarea parametrilor, gestionarea structurii directoarelor, calculul căilor relative și identificarea diferențelor între fișiere.

▶️ Compilare
- Pentru aplicația principală (Task 1-4): 
gcc tema.c task1.c task2.c task3.c task4.c -o tema

- Pentru utilitarul de verificare a dependențelor (Bonus):
gcc util_lib.c -o util_lib


📂 Task 1 – Parsare Argumente și Opțiuni

📘 Descriere:
Imită comportamentul comenzilor variadice din terminal. Primește un apel de tip ./program optiuni argumente flaguri și identifică:
- Argumentele poziționale
- Opțiunile (scurt/lung)
- Flagurile (fără valoare)

🧪 Apel:
./tema task1 --file=input.in --out=output.out [argumente] [--opt=val] [-f]

📂 Task 2 – Simulare `ls` cu Stivă

📘 Descriere:
Simulează comanda ls și afişează conținutul unui director, cu suport pentru:
- -h / --human-readable
- -r / --recursive
- -d / --directory

🧪 Apel:
./tema task2 <cale_director> [d|h|r|rh] --out=output.out

📂 Task 3 – Calcul Cale Relativă între Directoare

📘 Descriere:
Imită comanda cd pentru navigare în arborele de directoare. Construiește un arbore și determină calea relativă între două directoare absolute.

🧪 Apel:
./tema task3 /cale/sursa /cale/destinatie --out=output.out

📂 Task 4 – Comparare Fișiere (Diff între Linii)

📘 Descriere:
Simulează comanda diff. Compară două fișiere linie cu linie și calculează:
- Delete (D)
- Insert (I)
- Replace (R)

🧪 Apel:
./tema task4 file1.txt file2.txt --out=output.txt

📂 Bonus – Verificator de Dependențe (Include-uri)

📘 Descriere:
Un utilitar separat (`util_lib.c`) care parcurge un director și analizează fișierele sursă C/C++ (`.c`, `.h`). 
Acesta identifică posibile probleme de arhitectură în cod detectând buclele de includere (circular dependencies)  și importurile redundante.
- Loop Detection: Detectează dacă fișierele se includ reciproc într-o buclă infinită (ex: A include B, care include A).
- Double Includes: Cu ajutorul flag-ului `--full`, afișează avertismente (WARNING) dacă un anumit fișier header este inclus de mai multe ori în mod redundant în fișiere diferite.

🧪 Apel:
./util_lib <cale_director> [--full]
