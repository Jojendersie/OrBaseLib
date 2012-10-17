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

char* TEST_STRINGS[] = {"der", "die", "und", "in", "den", "von", "zu", "das", "mit", "sich", "des", "auf", "für", "ist", "im", "dem", "nicht", "ein", "Die", "eine", "als", "auch", "es", "an", "werden", "aus", "er", "hat", "daß", "sie", "nach", "wird",
"bei", "einer", "Der", "um", "am", "sind", "noch", "wie", "einem", "über", "einen", "Das", "so", "Sie", "zum", "war", "haben", "nur", "oder", "aber", "vor", "zur", "bis", "mehr", "durch", "man", "sein", "wurde", "sei", "In",
"Prozent", "hatte", "kann", "gegen", "vom", "können", "schon", "wenn", "habe", "seine", "Mark", "ihre", "dann", "unter", "wir", "soll", "ich", "eines", "Es", "Jahr", "zwei", "Jahren", "diese", "dieser", "wieder", "keine", "Uhr",
"seiner", "worden", "Und", "will", "zwischen", "Im", "immer", "Millionen", "Ein", "was", "sagte", "Er", "gibt", "alle", "DM", "diesem", "seit", "muß", "wurden", "beim", "doch", "jetzt", "waren", "drei", "Jahre", "Mit", "neue",
"neuen", "damit", "bereits", "da", "Auch", "ihr", "seinen", "müssen", "ab", "ihrer", "Nach", "ohne", "sondern", "selbst", "ersten", "nun", "etwa", "Bei", "heute", "ihren", "weil", "ihm", "seien", "Menschen", "Deutschland",
"anderen", "werde", "Ich", "sagt", "Wir", "Eine", "rund", "Für", "Aber", "ihn", "Ende", "jedoch", "Zeit", "sollen", "ins", "Wenn", "So", "seinem", "uns", "Stadt", "geht", "Doch", "sehr", "hier", "ganz", "erst", "wollen", "Berlin",
"vor allem", "sowie", "hatten", "kein", "deutschen", "machen", "lassen", "Als", "Unternehmen", "andere", "ob", "dieses", "steht", "dabei", "wegen", "weiter", "denn", "beiden", "einmal", "etwas", "Wie", "nichts", "allerdings",
"vier", "gut", "viele", "wo", "viel", "dort", "alles", "Auf", "wäre", "SPD", "kommt", "vergangenen", "denen", "fast", "fünf", "könnte", "nicht nur", "hätten", "Frau", "Am", "dafür", "kommen", "diesen", "letzten", "zwar", "Diese",
"großen", "dazu", "Von", "Mann", "Da", "sollte", "würde", "also", "bisher", "Leben", "Milliarden", "Welt", "Regierung", "konnte", "ihrem", "Frauen", "während", "Land", "zehn", "würden", "stehen", "ja", "USA", "heißt", "dies",
"zurück", "Kinder", "dessen", "ihnen", "deren", "sogar", "Frage", "gewesen", "erste", "gab", "liegt", "gar", "davon", "gestern", "geben", "Teil", "Polizei", "dass", "hätte", "eigenen", "kaum", "sieht", "große", "Denn", "weitere",
"Was", "sehen", "macht", "Angaben", "weniger", "gerade", "läßt", "Geld", "München", "deutsche", "allen", "darauf", "wohl", "später", "könne", "deshalb", "aller", "kam", "Arbeit", "mich", "gegenüber", "nächsten", "bleibt", "wenig",
"lange", "gemacht", "Wer", "Dies", "Fall", "mir", "gehen", "Berliner", "mal", "Weg", "CDU", "wollte", "sechs", "keinen", "Woche", "dagegen", "alten", "möglich", "gilt", "erklärte", "müsse", "Dabei", "könnten", "Geschichte",
"zusammen", "finden", "Tag", "Art", "erhalten", "Man", "Dollar", "Wochen", "jeder", "nie", "bleiben", "besonders", "Jahres", "Deutschen", "Den", "Zu", "zunächst", "derzeit", "allein", "deutlich", "Entwicklung", "weiß", "einige",
"sollten", "Präsident", "geworden", "statt", "Bonn", "Platz", "inzwischen", "Nur", "Freitag", "Um", "pro", "seines", "Damit", "Montag", "Europa", "schließlich", "Sonntag", "einfach", "gehört", "eher", "oft", "Zahl", "neben",
"hält", "weit", "Partei", "meisten", "Thema", "zeigt", "Politik", "Aus", "zweiten", "Januar", "insgesamt", "je", "mußte", "Anfang", "hinter", "ebenfalls", "ging", "Mitarbeiter", "darüber", "vielen", "Ziel", "darf", "Seite", "fest",
"hin", "erklärt", "Namen", "Haus", "An", "Frankfurt", "Gesellschaft", "Mittwoch", "damals", "Dienstag", "Hilfe", "Mai", "Markt", "Seit", "Tage", "Donnerstag", "halten", "gleich", "nehmen", "solche", "Entscheidung", "besser",
"alte", "Leute", "Ergebnis", "Samstag", "Daß", "sagen", "System", "März", "tun", "Monaten", "kleinen", "lang", "Nicht", "knapp", "bringen", "wissen", "Kosten", "Erfolg", "bekannt", "findet", "daran", "künftig", "wer", "acht",
"Grünen", "schnell", "Grund", "scheint", "Zukunft", "Stuttgart", "bin", "liegen", "politischen", "Gruppe", "Rolle", "stellt", "Juni", "sieben", "September", "nämlich", "Männer", "Oktober", "Mrd", "überhaupt", "eigene", "Dann",
"gegeben", "Außerdem", "Stunden", "eigentlich", "Meter", "ließ", "Probleme", "vielleicht", "ebenso", "Bereich", "zum Beispiel", "Bis", "Höhe", "Familie", "Während", "Bild", "Ländern", "Informationen", "Frankreich", "Tagen",
"schwer", "zuvor", "Vor", "genau", "April", "stellen", "neu", "erwartet", "Hamburg", "sicher", "führen", "Mal", "Über", "mehrere", "Wirtschaft", "Mio", "Programm", "offenbar", "Hier", "weiteren", "natürlich", "konnten", "stark",
"Dezember", "Juli", "ganze", "kommenden", "Kunden", "bekommen", "eben", "kleine", "trotz", "wirklich", "Lage", "Länder", "leicht", "gekommen", "Spiel", "laut", "November", "kurz", "politische", "führt", "innerhalb", "unsere",
"meint", "immer wieder", "Form", "Münchner", "AG", "anders", "ihres", "völlig", "beispielsweise", "gute", "bislang", "August", "Hand", "jede", "GmbH", "Film", "Minuten", "erreicht", "beide", "Musik", "Kritik", "Mitte", "Verfügung",
"Buch", "dürfen", "Unter", "jeweils", "einigen", "Zum", "Umsatz", "spielen", "Daten", "welche", "müßten", "hieß", "paar", "nachdem", "Kunst", "Euro", "gebracht", "Problem", "Noch", "jeden", "Ihre", "Sprecher", "recht", "erneut",
"längst", "europäischen", "Sein", "Eltern", "Beginn", "besteht", "Seine", "mindestens", "machte", "Jetzt", "bietet", "außerdem", "Bürger", "Trainer", "bald", "Deutsche", "Schon", "Fragen", "klar", "Durch", "Seiten", "gehören",
"Dort", "erstmals", "Februar", "zeigen", "Titel", "Stück", "größten", "FDP", "setzt", "Wert", "Frankfurter", "Staat", "möchte", "daher", "wolle", "Bundesregierung", "lediglich", "Nacht", "Krieg", "Opfer", "Tod", "nimmt", "Firma",
"zuletzt", "Werk", "hohen", "leben", "unter anderem", "Dieser", "Kirche", "weiterhin", "gebe", "gestellt", "Mitglieder", "Rahmen", "zweite", "Paris", "Situation", "gefunden", "Wochenende", "internationalen", "Wasser", "Recht",
"sonst", "stand", "Hälfte", "Möglichkeit", "versucht", "blieb", "junge", "Mehrheit", "Straße", "Sache", "arbeiten", "Monate", "Mutter", "berichtet", "letzte", "Gericht", "wollten", "Ihr", "zwölf", "zumindest", "Wahl", "genug",
"Weise", "Vater", "Bericht", "amerikanischen", "hoch", "beginnt", "Wort", "obwohl", "Kopf", "spielt", "Interesse", "Westen", "verloren", "Preis", "Erst", "jedem", "erreichen", "setzen", "spricht", "früher", "teilte", "Landes",
"zudem", "einzelnen", "bereit", "Blick", "Druck", "Bayern", "Kilometer", "gemeinsam", "Bedeutung", "Chance", "Politiker", "Dazu", "Zwei", "besten", "Ansicht", "endlich", "Stelle", "direkt", "Beim", "Bevölkerung", "Viele",
"solchen", "Alle", "solle", "jungen", "Einsatz", "richtig", "größte", "sofort", "neuer", "ehemaligen", "unserer", "dürfte", "schaffen", "Augen", "Rußland", "Internet", "Allerdings", "Raum", "Mannschaft", "neun", "kamen",
"Ausstellung", "Zeiten", "Dem", "einzige", "meine", "Nun", "Verfahren", "Angebot", "Richtung", "Projekt", "niemand", "Kampf", "weder", "tatsächlich", "Personen", "dpa", "Heute", "geführt", "Gespräch", "Kreis", "Hamburger",
"Schule", "guten", "Hauptstadt", "durchaus", "Zusammenarbeit", "darin", "Amt", "Schritt", "meist", "groß", "zufolge", "Sprache", "Region", "Punkte", "Vergleich", "genommen", "gleichen", "du", "Ob", "Soldaten", "Universität",
"verschiedenen", "Kollegen", "neues", "Bürgermeister", "Angst", "stellte", "Sommer", "danach", "anderer", "gesagt", "Sicherheit", "Macht", "Bau", "handelt", "Folge", "Bilder", "lag", "Osten", "Handel", "sprach", "Aufgabe", "Chef",
"frei", "dennoch", "DDR", "hohe", "Firmen", "bzw", "Koalition", "Mädchen", "Zur", "entwickelt", "fand", "Diskussion", "bringt", "Deshalb", "Hause", "Gefahr", "per", "zugleich", "früheren", "dadurch", "ganzen", "abend", "erzählt",
"Streit", "Vergangenheit", "Parteien", "Verhandlungen", "jedenfalls", "gesehen", "französischen", "Trotz", "darunter", "Spieler", "forderte", "Beispiel", "Meinung", "wenigen", "Publikum", "sowohl", "meinte", "mag", "Auto",
"Lösung", "Boden", "Einen", "Präsidenten", "hinaus", "Zwar", "verletzt", "weltweit", "Sohn", "bevor", "Peter", "mußten", "keiner", "Produktion", "Ort", "braucht", "Zusammenhang", "Kind", "Verein", "sprechen", "Aktien",
"gleichzeitig", "London", "sogenannten", "Richter", "geplant", "Italien", "Mittel", "her", "freilich", "Mensch", "großer", "Bonner", "wenige", "öffentlichen", "Unterstützung", "dritten", "nahm", "Bundesrepublik", "Arbeitsplätze",
"bedeutet", "Feld", "Dr.", "Bank", "oben", "gesetzt", "Ausland", "Ministerpräsident", "Vertreter", "z.B.", "jedes", "ziehen", "Parlament", "berichtete", "Dieses", "China", "aufgrund", "Stellen", "warum", "Kindern", "heraus",
"heutigen", "Anteil", "Herr", "Öffentlichkeit", "Abend", "Selbst", "Liebe", "Neben", "rechnen", "fällt", "New York", "Industrie", "WELT", "Stuttgarter", "wären", "Vorjahr", "Sicht", "Idee", "Banken", "verlassen", "Leiter", "Bühne",
"insbesondere", "offen", "stets", "Theater", "ändern", "entschieden", "Staaten", "Experten", "Gesetz", "Geschäft", "Tochter", "angesichts", "gelten", "Mehr", "erwarten", "läuft", "fordert", "Japan", "Sieg", "Ist", "Stimmen",
"wählen", "russischen", "gewinnen", "CSU", "bieten", "Nähe", "jährlich", "Bremen", "Schüler", "Rede", "Funktion", "Zuschauer", "hingegen", "anderes", "Führung", "Besucher", "Drittel", "Moskau", "immerhin", "Vorsitzende", "Urteil",
"Schließlich", "Kultur", "betonte", "mittlerweile", "Saison", "Konzept", "suchen", "Zahlen", "Roman", "Gewalt", "Köln", "gesamte", "indem", "EU", "Stunde", "ehemalige", "Auftrag", "entscheiden", "genannt", "tragen", "Börse",
"langen", "häufig", "Chancen", "Vor allem", "Position", "alt", "Luft", "Studenten", "übernehmen", "stärker", "ohnehin", "zeigte", "geplanten", "Reihe", "darum", "verhindern", "begann", "Medien", "verkauft", "Minister", "wichtig",
"amerikanische", "sah", "gesamten", "einst", "verwendet", "vorbei", "Behörden", "helfen", "Folgen", "bezeichnet", "Weil", "Ihnen", "zur Zeit", "voll", "deutscher", "Worten", "plötzlich", "müßte", "Vertrag", "Staatsanwaltschaft",
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