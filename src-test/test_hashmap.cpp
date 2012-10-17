// ******************************************************************************** //
// test_hashmap.cpp																	//
// ================																	//
// This file is part of the OrBaseLib Unit Test.									//
//																					//
// Author: Johannes Jendersie														//
// Content: Testing of the class OrE::ADT::Hashmap. Tested are all different		//
//	Modi.
//																					//
// ******************************************************************************** //

#include <iostream>
#include "../include/OrBaseLib.h"
#include "../include/OrDebug.h"

void test_hminteger( OrE::ADT::HashMap& _HM, const char* _pcMode );
void test_hmstring( OrE::ADT::HashMap& _HM, const char* _pcMode );

char* TEST_STRINGS[] = {"der", "die", "und", "in", "den", "von", "zu", "das", "mit", "sich", "des", "auf", "f�r", "ist", "im", "dem", "nicht", "ein", "Die", "eine", "als", "auch", "es", "an", "werden", "aus", "er", "hat", "da�", "sie", "nach", "wird",
"bei", "einer", "Der", "um", "am", "sind", "noch", "wie", "einem", "�ber", "einen", "Das", "so", "Sie", "zum", "war", "haben", "nur", "oder", "aber", "vor", "zur", "bis", "mehr", "durch", "man", "sein", "wurde", "sei", "In",
"Prozent", "hatte", "kann", "gegen", "vom", "k�nnen", "schon", "wenn", "habe", "seine", "Mark", "ihre", "dann", "unter", "wir", "soll", "ich", "eines", "Es", "Jahr", "zwei", "Jahren", "diese", "dieser", "wieder", "keine", "Uhr",
"seiner", "worden", "Und", "will", "zwischen", "Im", "immer", "Millionen", "Ein", "was", "sagte", "Er", "gibt", "alle", "DM", "diesem", "seit", "mu�", "wurden", "beim", "doch", "jetzt", "waren", "drei", "Jahre", "Mit", "neue",
"neuen", "damit", "bereits", "da", "Auch", "ihr", "seinen", "m�ssen", "ab", "ihrer", "Nach", "ohne", "sondern", "selbst", "ersten", "nun", "etwa", "Bei", "heute", "ihren", "weil", "ihm", "seien", "Menschen", "Deutschland",
"anderen", "werde", "Ich", "sagt", "Wir", "Eine", "rund", "F�r", "Aber", "ihn", "Ende", "jedoch", "Zeit", "sollen", "ins", "Wenn", "So", "seinem", "uns", "Stadt", "geht", "Doch", "sehr", "hier", "ganz", "erst", "wollen", "Berlin",
"vor allem", "sowie", "hatten", "kein", "deutschen", "machen", "lassen", "Als", "Unternehmen", "andere", "ob", "dieses", "steht", "dabei", "wegen", "weiter", "denn", "beiden", "einmal", "etwas", "Wie", "nichts", "allerdings",
"vier", "gut", "viele", "wo", "viel", "dort", "alles", "Auf", "w�re", "SPD", "kommt", "vergangenen", "denen", "fast", "f�nf", "k�nnte", "nicht nur", "h�tten", "Frau", "Am", "daf�r", "kommen", "diesen", "letzten", "zwar", "Diese",
"gro�en", "dazu", "Von", "Mann", "Da", "sollte", "w�rde", "also", "bisher", "Leben", "Milliarden", "Welt", "Regierung", "konnte", "ihrem", "Frauen", "w�hrend", "Land", "zehn", "w�rden", "stehen", "ja", "USA", "hei�t", "dies",
"zur�ck", "Kinder", "dessen", "ihnen", "deren", "sogar", "Frage", "gewesen", "erste", "gab", "liegt", "gar", "davon", "gestern", "geben", "Teil", "Polizei", "dass", "h�tte", "eigenen", "kaum", "sieht", "gro�e", "Denn", "weitere",
"Was", "sehen", "macht", "Angaben", "weniger", "gerade", "l��t", "Geld", "M�nchen", "deutsche", "allen", "darauf", "wohl", "sp�ter", "k�nne", "deshalb", "aller", "kam", "Arbeit", "mich", "gegen�ber", "n�chsten", "bleibt", "wenig",
"lange", "gemacht", "Wer", "Dies", "Fall", "mir", "gehen", "Berliner", "mal", "Weg", "CDU", "wollte", "sechs", "keinen", "Woche", "dagegen", "alten", "m�glich", "gilt", "erkl�rte", "m�sse", "Dabei", "k�nnten", "Geschichte",
"zusammen", "finden", "Tag", "Art", "erhalten", "Man", "Dollar", "Wochen", "jeder", "nie", "bleiben", "besonders", "Jahres", "Deutschen", "Den", "Zu", "zun�chst", "derzeit", "allein", "deutlich", "Entwicklung", "wei�", "einige",
"sollten", "Pr�sident", "geworden", "statt", "Bonn", "Platz", "inzwischen", "Nur", "Freitag", "Um", "pro", "seines", "Damit", "Montag", "Europa", "schlie�lich", "Sonntag", "einfach", "geh�rt", "eher", "oft", "Zahl", "neben",
"h�lt", "weit", "Partei", "meisten", "Thema", "zeigt", "Politik", "Aus", "zweiten", "Januar", "insgesamt", "je", "mu�te", "Anfang", "hinter", "ebenfalls", "ging", "Mitarbeiter", "dar�ber", "vielen", "Ziel", "darf", "Seite", "fest",
"hin", "erkl�rt", "Namen", "Haus", "An", "Frankfurt", "Gesellschaft", "Mittwoch", "damals", "Dienstag", "Hilfe", "Mai", "Markt", "Seit", "Tage", "Donnerstag", "halten", "gleich", "nehmen", "solche", "Entscheidung", "besser",
"alte", "Leute", "Ergebnis", "Samstag", "Da�", "sagen", "System", "M�rz", "tun", "Monaten", "kleinen", "lang", "Nicht", "knapp", "bringen", "wissen", "Kosten", "Erfolg", "bekannt", "findet", "daran", "k�nftig", "wer", "acht",
"Gr�nen", "schnell", "Grund", "scheint", "Zukunft", "Stuttgart", "bin", "liegen", "politischen", "Gruppe", "Rolle", "stellt", "Juni", "sieben", "September", "n�mlich", "M�nner", "Oktober", "Mrd", "�berhaupt", "eigene", "Dann",
"gegeben", "Au�erdem", "Stunden", "eigentlich", "Meter", "lie�", "Probleme", "vielleicht", "ebenso", "Bereich", "zum Beispiel", "Bis", "H�he", "Familie", "W�hrend", "Bild", "L�ndern", "Informationen", "Frankreich", "Tagen",
"schwer", "zuvor", "Vor", "genau", "April", "stellen", "neu", "erwartet", "Hamburg", "sicher", "f�hren", "Mal", "�ber", "mehrere", "Wirtschaft", "Mio", "Programm", "offenbar", "Hier", "weiteren", "nat�rlich", "konnten", "stark",
"Dezember", "Juli", "ganze", "kommenden", "Kunden", "bekommen", "eben", "kleine", "trotz", "wirklich", "Lage", "L�nder", "leicht", "gekommen", "Spiel", "laut", "November", "kurz", "politische", "f�hrt", "innerhalb", "unsere",
"meint", "immer wieder", "Form", "M�nchner", "AG", "anders", "ihres", "v�llig", "beispielsweise", "gute", "bislang", "August", "Hand", "jede", "GmbH", "Film", "Minuten", "erreicht", "beide", "Musik", "Kritik", "Mitte", "Verf�gung",
"Buch", "d�rfen", "Unter", "jeweils", "einigen", "Zum", "Umsatz", "spielen", "Daten", "welche", "m��ten", "hie�", "paar", "nachdem", "Kunst", "Euro", "gebracht", "Problem", "Noch", "jeden", "Ihre", "Sprecher", "recht", "erneut",
"l�ngst", "europ�ischen", "Sein", "Eltern", "Beginn", "besteht", "Seine", "mindestens", "machte", "Jetzt", "bietet", "au�erdem", "B�rger", "Trainer", "bald", "Deutsche", "Schon", "Fragen", "klar", "Durch", "Seiten", "geh�ren",
"Dort", "erstmals", "Februar", "zeigen", "Titel", "St�ck", "gr��ten", "FDP", "setzt", "Wert", "Frankfurter", "Staat", "m�chte", "daher", "wolle", "Bundesregierung", "lediglich", "Nacht", "Krieg", "Opfer", "Tod", "nimmt", "Firma",
"zuletzt", "Werk", "hohen", "leben", "unter anderem", "Dieser", "Kirche", "weiterhin", "gebe", "gestellt", "Mitglieder", "Rahmen", "zweite", "Paris", "Situation", "gefunden", "Wochenende", "internationalen", "Wasser", "Recht",
"sonst", "stand", "H�lfte", "M�glichkeit", "versucht", "blieb", "junge", "Mehrheit", "Stra�e", "Sache", "arbeiten", "Monate", "Mutter", "berichtet", "letzte", "Gericht", "wollten", "Ihr", "zw�lf", "zumindest", "Wahl", "genug",
"Weise", "Vater", "Bericht", "amerikanischen", "hoch", "beginnt", "Wort", "obwohl", "Kopf", "spielt", "Interesse", "Westen", "verloren", "Preis", "Erst", "jedem", "erreichen", "setzen", "spricht", "fr�her", "teilte", "Landes",
"zudem", "einzelnen", "bereit", "Blick", "Druck", "Bayern", "Kilometer", "gemeinsam", "Bedeutung", "Chance", "Politiker", "Dazu", "Zwei", "besten", "Ansicht", "endlich", "Stelle", "direkt", "Beim", "Bev�lkerung", "Viele",
"solchen", "Alle", "solle", "jungen", "Einsatz", "richtig", "gr��te", "sofort", "neuer", "ehemaligen", "unserer", "d�rfte", "schaffen", "Augen", "Ru�land", "Internet", "Allerdings", "Raum", "Mannschaft", "neun", "kamen",
"Ausstellung", "Zeiten", "Dem", "einzige", "meine", "Nun", "Verfahren", "Angebot", "Richtung", "Projekt", "niemand", "Kampf", "weder", "tats�chlich", "Personen", "dpa", "Heute", "gef�hrt", "Gespr�ch", "Kreis", "Hamburger",
"Schule", "guten", "Hauptstadt", "durchaus", "Zusammenarbeit", "darin", "Amt", "Schritt", "meist", "gro�", "zufolge", "Sprache", "Region", "Punkte", "Vergleich", "genommen", "gleichen", "du", "Ob", "Soldaten", "Universit�t",
"verschiedenen", "Kollegen", "neues", "B�rgermeister", "Angst", "stellte", "Sommer", "danach", "anderer", "gesagt", "Sicherheit", "Macht", "Bau", "handelt", "Folge", "Bilder", "lag", "Osten", "Handel", "sprach", "Aufgabe", "Chef",
"frei", "dennoch", "DDR", "hohe", "Firmen", "bzw", "Koalition", "M�dchen", "Zur", "entwickelt", "fand", "Diskussion", "bringt", "Deshalb", "Hause", "Gefahr", "per", "zugleich", "fr�heren", "dadurch", "ganzen", "abend", "erz�hlt",
"Streit", "Vergangenheit", "Parteien", "Verhandlungen", "jedenfalls", "gesehen", "franz�sischen", "Trotz", "darunter", "Spieler", "forderte", "Beispiel", "Meinung", "wenigen", "Publikum", "sowohl", "meinte", "mag", "Auto",
"L�sung", "Boden", "Einen", "Pr�sidenten", "hinaus", "Zwar", "verletzt", "weltweit", "Sohn", "bevor", "Peter", "mu�ten", "keiner", "Produktion", "Ort", "braucht", "Zusammenhang", "Kind", "Verein", "sprechen", "Aktien",
"gleichzeitig", "London", "sogenannten", "Richter", "geplant", "Italien", "Mittel", "her", "freilich", "Mensch", "gro�er", "Bonner", "wenige", "�ffentlichen", "Unterst�tzung", "dritten", "nahm", "Bundesrepublik", "Arbeitspl�tze",
"bedeutet", "Feld", "Dr.", "Bank", "oben", "gesetzt", "Ausland", "Ministerpr�sident", "Vertreter", "z.B.", "jedes", "ziehen", "Parlament", "berichtete", "Dieses", "China", "aufgrund", "Stellen", "warum", "Kindern", "heraus",
"heutigen", "Anteil", "Herr", "�ffentlichkeit", "Abend", "Selbst", "Liebe", "Neben", "rechnen", "f�llt", "New York", "Industrie", "WELT", "Stuttgarter", "w�ren", "Vorjahr", "Sicht", "Idee", "Banken", "verlassen", "Leiter", "B�hne",
"insbesondere", "offen", "stets", "Theater", "�ndern", "entschieden", "Staaten", "Experten", "Gesetz", "Gesch�ft", "Tochter", "angesichts", "gelten", "Mehr", "erwarten", "l�uft", "fordert", "Japan", "Sieg", "Ist", "Stimmen",
"w�hlen", "russischen", "gewinnen", "CSU", "bieten", "N�he", "j�hrlich", "Bremen", "Sch�ler", "Rede", "Funktion", "Zuschauer", "hingegen", "anderes", "F�hrung", "Besucher", "Drittel", "Moskau", "immerhin", "Vorsitzende", "Urteil",
"Schlie�lich", "Kultur", "betonte", "mittlerweile", "Saison", "Konzept", "suchen", "Zahlen", "Roman", "Gewalt", "K�ln", "gesamte", "indem", "EU", "Stunde", "ehemalige", "Auftrag", "entscheiden", "genannt", "tragen", "B�rse",
"langen", "h�ufig", "Chancen", "Vor allem", "Position", "alt", "Luft", "Studenten", "�bernehmen", "st�rker", "ohnehin", "zeigte", "geplanten", "Reihe", "darum", "verhindern", "begann", "Medien", "verkauft", "Minister", "wichtig",
"amerikanische", "sah", "gesamten", "einst", "verwendet", "vorbei", "Beh�rden", "helfen", "Folgen", "bezeichnet", "Weil", "Ihnen", "zur Zeit", "voll", "deutscher", "Worten", "pl�tzlich", "m��te", "Vertrag", "Staatsanwaltschaft",
"Monat", "Oder", "Herbst", "Israel", "zahlen", "Zeitung", "Grenzen", "Wissenschaftler", "Partner", "Patienten", "nutzen", "Bund", "setzte", "Betrieb"};

void test_hashmap()
{
	// Test all modi. Compare collisions and total time for insertions and searches.
	// Collisions can only be counted in debug mode.
	// First test with 10000 random numbers. Numbers can be used more than one time the
	// key is the loop index.
	OrE::ADT::HashMap MyHM1( 128, OrE::ADT::HashMapMode::HM_NO_RESIZE );
	OrE::ADT::HashMap MyHM2( 128, OrE::ADT::HashMapMode::HM_PREFER_SIZE );
	OrE::ADT::HashMap MyHM3( 128, OrE::ADT::HashMapMode::HM_RESIZE_MODERATE );
	OrE::ADT::HashMap MyHM4( 128, OrE::ADT::HashMapMode::HM_PREFER_PERFORMANCE );
	
	std::cout << "******** Start test OrE::ADT::Hashmap ********\n";
	std::cout << "\tAll test uses hashmaps with the initial size of 128 elments\n";

	test_hminteger( MyHM1, "NO_RESIZE" );
	test_hminteger( MyHM2, "PREFER_SIZE" );
	test_hminteger( MyHM3, "RESIZE_MODERATE" );
	test_hminteger( MyHM4, "PREFER_PERFORMANCE" );

	// Second test with the 1024 mostly used german words.
	OrE::ADT::HashMap MySHM1( 128, OrE::ADT::HashMapMode( OrE::ADT::HashMapMode::HM_NO_RESIZE | OrE::ADT::HashMapMode::HM_USE_STRING_MODE ) );
	OrE::ADT::HashMap MySHM2( 128, OrE::ADT::HashMapMode( OrE::ADT::HashMapMode::HM_PREFER_SIZE | OrE::ADT::HashMapMode::HM_USE_STRING_MODE ) );
	OrE::ADT::HashMap MySHM3( 128, OrE::ADT::HashMapMode( OrE::ADT::HashMapMode::HM_RESIZE_MODERATE | OrE::ADT::HashMapMode::HM_USE_STRING_MODE ) );
	OrE::ADT::HashMap MySHM4( 128, OrE::ADT::HashMapMode( OrE::ADT::HashMapMode::HM_PREFER_PERFORMANCE | OrE::ADT::HashMapMode::HM_USE_STRING_MODE ) );

	std::cout << "\tThe following tests use the STRING_MODE.\n\tThe testdata is a list of the 1024 mostly used german words.\n";
	test_hmstring( MySHM1, "NO_RESIZE" );
	test_hmstring( MySHM2, "PREFER_SIZE" );
	test_hmstring( MySHM3, "RESIZE_MODERATE" );
	test_hmstring( MySHM4, "PREFER_PERFORMANCE" );

	std::cout << '\n';
}

void test_hminteger( OrE::ADT::HashMap& _HM, const char* _pcMode )
{
	OrE::Utils::TimeQuerySlot Time0;

	// Add primitive data (intergers) to the map
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<10000; ++i )
		_HM.Insert( (void*)OrE::Algorithm::Rand(0,1000), i );

	std::cout << '\t' << _pcMode << " - 10000 insertions: " << OrE::Utils::TimeQuery( Time0 ) << " s\n";
#ifdef _DEBUG
	std::cout << "\t\t- " << (_HM.m_dwCollsionCounter/10000.0f) << " Collsions/Insertion" <<  '\n';
#endif

	// Time reset
	OrE::Utils::TimeQuery( Time0 );

	// Iterate over all 10000 elements and change data
	OrE::ADT::Iterator<OrE::ADT::ADTElement> It(&_HM);
	while(++It) {
		It->pObject = (void*)((int)It->pObject + 500);
	}

	std::cout << "\t\t- Iteration over 10000 elements: " << OrE::Utils::TimeQuery( Time0 ) << " s\n";

	// Search 1000 random elements (in map and not in map 50%)
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<1000; ++i )
		_HM.Search( OrE::Algorithm::Rand(0,20000) );

	std::cout << "\t\t- Time to search for 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Delete 1000 elements
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<1000; ++i )
		_HM.Delete( OrE::Algorithm::Rand(0,10000) );

	std::cout << "\t\t- Time to remove 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	_HM.Clear();
}

void test_hmstring( OrE::ADT::HashMap& _HM, const char* _pcMode )
{
	OrE::Utils::TimeQuerySlot Time0;

	// Add primitive data (intergers) to the map, use string keys
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<1024; ++i )
		_HM.Insert( (void*)OrE::Algorithm::Rand(0,1000), TEST_STRINGS[i] );

	std::cout << '\t' << _pcMode << " - 1024 insertions: " << OrE::Utils::TimeQuery( Time0 ) << " s\n";
#ifdef _DEBUG
	std::cout << "\t\t- " << (_HM.m_dwCollsionCounter/1024.0f) << " Collsions/Insertion" <<  '\n';
#endif

	// Time reset
	OrE::Utils::TimeQuery( Time0 );

	// Iterate over all 1024 elements and change data
	OrE::ADT::Iterator<OrE::ADT::ADTElement> It(&_HM);
	while(++It) {
		It->pObject = (void*)((int)It->pObject + 500);
	}

	std::cout << "\t\t- Iteration over 1024 elements: " << OrE::Utils::TimeQuery( Time0 ) << " s\n";

	// Search 1000 random elements (in map and not in map 50%)
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<1000; ++i )
	{
		int iEntry = OrE::Algorithm::Rand(0,2047);
		_HM.Search( (iEntry<1024) ? TEST_STRINGS[iEntry] : "NotInMap" );
	}

	std::cout << "\t\t- Time to search for 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';

	// Delete 1000 elements
	OrE::Utils::TimeQuery( Time0 );
	for( int i=0; i<1000; ++i )
		_HM.Delete( TEST_STRINGS[OrE::Algorithm::Rand(0,1023)] );

	std::cout << "\t\t- Time to remove 1000 elements: " << OrE::Utils::TimeQuery( Time0 ) << '\n';
	_HM.Clear();
}