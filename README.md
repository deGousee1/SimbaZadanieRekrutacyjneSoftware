Instrukcja uruchomienia:

1. Pobierz plik zip projektu i umieść go w folderze docelowym.
2. Wypakuj pliki w folderze docelowym.
3. Otwórz wypakowany folder projektu w swoim IDE.
4. Uruchom program z IDE.


Struktura projektu:

CMakeLists.txt		# Plik konfiguracyjny
TelemetryBlackBox.h	# Deklaracja TelemetryBlackBox.cpp
TelemetryBlackBox.cpp	# Implementacja logiki zadania (główna treść zadania)
Test.h			# Deklaracja klasy testującej
Test.cpp		# Implementacja poszczególnych testów
main.cpp		# Obsługa uruchamiania poszczególnych testów
README.md		# Dokumentacja i instrukcja projektu


Opis które pliki za co odpowiadają:

CMakeLists.txt 		- Plik konfiguracyjny i służy do zbudowania programu.
TelemetryBlackBox.h 	- Deklaruje funkcje, których dokładny kod znajduje się w TelemetryBlackBox.cpp
TelemetryBlackBox.cpp 	- Odpowiada za logikę funkcji. Odbiera i zapisuje dane o stanie rakiety i pakuje je w 32 bitową ramkę.
			  Logika funkcji obsługuje te przypadki wymienione w zadaniu. W tym pliku znajduje się też bufor kołowy i logika jego obsługi.
Test.h			- Deklaruje funkcje z pliku Test.cpp.
Test.cpp		- W tym pliku znajduje się logika kilku różnych testów (dwa testy symulowanego lotu, test bufora i test flagi błędu checksum).
main.cpp		- Main pełni rolę interfejsu przez który uruchamia się odpowiednie testy.