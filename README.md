# Tema 2

# Descriere

Acest proiect este o aplicație scrisă în limbajul C care simulează un mecanism de indexare pe baza cuvintelor-cheie asociate acestora.

# Structuri de date folosite

In acest proiect, am folosit mai multe structuri de date pentru a putea stoca informațiile oferite. O listă dublu înlănțuită memorează un identificator unic și un scor de relevanță pentru fișierele date. Cuvintele-cheie asociate sunt stocate cu ajutorul unui arbore multicăi de regăsire. Nodul terminal al fiecărui cuvânt conține o listă de referințe unde se află pointeri către fiecare fișier care conține acel cuvânt-cheie și o variabilă 'ok' care indică dacă este terminal. Se mai folosește și un max-heap pentru a determina cele mai relevante k fișiere asociate unui cuvânt dat.
Structurile de date sunt trecute în fișierul var.h, împreună cu anteturile funcțiilor folosite pentru arbore, lista de referințe și pentru max-heap.

# Mecanismul de functionare

Informatiile sunt citite din fisierul de intrate indexare.in și sunt afișate in fisierul indexare.out. In fisier se afla diverse operații care pot fi efectuate asupra sistemului, urmate de informații unde este cazul:

1. ADD <id> <scor> <nr_cuvinte> <cuvinte-cheie> - Adaugă un fișier nou în lista de fișiere și cuvintele-cheie asociate în arbore. Dacă id-ul există deja, se afișează un mesaj corespunzător.

2. DEL <id> - Șterge fișierul cu identificatorul dat. Dacă nu există, se afișează mesajul NOT FOUND.

3. ADDKW <id> <cuvânt-cheie> - Adaugă un cuvânt-cheie nou unui fișier existent. În urma efectuării cu succes a operației se afișează mesajul OK, altfel se afișează NOT FOUND.

4. DELKW <id> <cuvânt-cheie> - Șterge un cuvân-cheie dintr-un fișier existent. Dacă fișierul nu există, operația eșuează și se afișează NOT FOUND. Dacă fișierul există deja și are deja asociat acel cuvânt-cheie, operația nu modifică sistemul, dar este considerată reușită și se afișează OK.

5. FIND <cuvânt-cheie> - Se afișează toate fișierele care conțin cuvântul dat în ordine lexicografică a identificatorilor.

6. TOPK <cuvânt-cheie> <k> -  Se determină cele mai relevante k fișiere asociate cuvântului dat, folosind un max-heap. Fișierele sunt ordonate în funcție de scorul de relevanță, iar dacă există două fișiere cu același scor, se vor adauga în heap în ordine lexicografică.

7. PRINT - Afișează toate cuvintele-cheie în ordine lexicografică, alături de numărul de fișiere asociate și identificatorii acelor fișiere în ordine lexicografică.

8. PREFIX <prefix>- Primește un prefix și trebuie să determine toate fișierele care conțin cel puțin un cuvânt-cheie ce începe cu prefixul dat. Rezultatele se afișează în ordine lexicografică.

Pe langa toate acestea, există funcții pentru gestionarea structurilor de date. Pentru lista de fișiere, se pot efectua operațiile: init_list, add_list, del_list, verify_list, free_list. Pentru arborele de regăsire, se folosesc funcțiile: init_node_trie, add_trie, search, del_file_trie, has_children, search_prefix, del_word_trie, verify_trie, print_trie, add_file, free_trie. Pentru lista de referințe, se pot efectua operatiile: add_refs, del_refs, sort_refs, free_refs. Pentru max-heap, se pot efectua operatiile: swap, add_heap, siftup, is_empty.

# Compilare și Rulare

Pentru a compila programul folosind `gcc`, rulați în terminal "make build", iar pentru a rula programul rulați "make run".

