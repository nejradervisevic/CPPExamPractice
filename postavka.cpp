#include <iostream>
using namespace std;
string crt = "\n---------------------------------------------------------------------------------\n";
string PORUKA_TELEFON = crt + "TELEFONE ISKLJUCITE I ODLOZITE U TORBU, DZEP ILI DRUGU LOKACIJU VAN DOHVATA.\n"
"CESTO SE NA TELEFONIMA (PRO)NALAZE PROGRAMSKI KODOVI KOJI MOGU BITI ISKORISTENI ZA\n"
"RJESAVANJE ISPITNOG ZADATKA, STO CE, U SLUCAJU PRONALASKA, BITI SANKCIONISANO.\n\n"
"SVI KOD KOJIH BUDE PRONADJEN TELEFON ILI NEKI DRUGI UREDJAJ KOJI MOZE SADRZAVATI\n"
"MATERIJAL ILI RJESENJA ISPITNIH ZADATAKA CE BITI UDALJENI SA ISPITA I BIT CE POKRENUT\n"
"POSTUPAK PROTIV NJIH" + crt;

string PORUKA_ISPIT = crt +
"0. PROVJERITE DA LI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE SA DINAMICKOM ALOKACIJOM MORAJU IMATI ISPRAVAN DESTRUKTOR\n"
"2. IZOSTAVLJANJE DESTRUKTORA ILI NJEGOVIH DIJELOVA BIT CE OZNACENO KAO TM\n"
"3. ATRIBUTI, METODE I PARAMETRI MORAJU BITI IDENTICNI ONIMA U TESTNOJ MAIN FUNKCIJI, \n"
" OSIM AKO POSTOJI JASNO OPISAN RAZLOG ZA MODIFIKACIJU\n"
"4. IZUZETKE BACAJTE SAMO TAMO GDJE JE IZRICITO NAGLASENO\n"
"5. SVE METODE KOJE SE POZIVAJU U MAIN-U MORAJU POSTOJATI.\n"
" AKO NEMATE ZELJENU IMPLEMENTACIJU, OSTAVITE PRAZNO TIJELO ILI VRATITE DEFAULT VRIJEDNOST\n"
"6. U MAIN FUNKCIJI MOZETE DODAVATI TESTNE PODATKE I POZIVE PO VLASTITOM IZBORU\n"
"7. TESTIRAJTE PROGRAM U OBA MODA (F5 i Ctrl+F5)" + crt;

char* AlocirajTekst(const char* tekst) {
	if (tekst == nullptr) return nullptr;
	size_t velicina = strlen(tekst) + 1;
	char* noviTekst = new char[velicina];
	strcpy_s(noviTekst, velicina, tekst);
	return noviTekst;
}
enum StatusNastanjivanja { NOVOPRIDOSLA, KARANTIN, PRILAGODJAVANJE, NASTANJENA };
const char* StatusNastanjivanjaNazivi[] = {
"NOVOPRIDOSLA", "KARANTIN", "PRILAGODJAVANJE", "NASTANJENA" };

template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	Kolekcija() : _trenutno(0) {
		_elementi1 = new T1[max];
		_elementi2 = new T2[max];
	}
	int GetTrenutno() const { return _trenutno; }
	const T1& GetElement1(int indeks) const { return _elementi1[indeks]; }
	const T2& GetElement2(int indeks) const { return _elementi2[indeks]; }
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	~Kolekcija() {
		delete[] _elementi1;
		delete[] _elementi2;
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
};
class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000,
		int sati = 0, int minute = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minute = new int(minute);
	}
	int GetGodina() const { return *_godina; }
	friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj)
	{
		// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM
		COUT << obj.ToString();
		return COUT;
	}
	~DatumVrijeme() {
		delete _dan;
		delete _mjesec;
		delete _godina;
		delete _sati;
		delete _minute;
		_dan = _mjesec = _godina = _sati = _minute = nullptr;
	}
};

class Pregled {
	string _opis;
	string _veterinar;
	double _cijena;
	int _trajanjeMinuta;
public:
	const string& GetOpis() const { return _opis; }
	const string& GetVeterinar() const { return _veterinar; }
	double GetCijena() const { return _cijena; }
	int GetTrajanjeMinuta() const { return _trajanjeMinuta; }
	friend ostream& operator<<(ostream& COUT, const Pregled& obj)
	{
		/*
		ToString vraca podatke u formatu :
		opis | veterinar | cijena KM | trajanje min
		Vakcinacija | Dr. Haris Hadzic | 65.50 KM | 20 min
		*/
		COUT << obj.ToString();
		return COUT;
	}
};
class Cuvar {
	string _imePrezime;
	string _email;
	string _telefon;
public:
	const string& GetImePrezime() const { return _imePrezime; }
	const string& GetEmail() const { return _email; }
	const string& GetTelefon() const { return _telefon; }
	friend ostream& operator<<(ostream& COUT, const Cuvar& obj) {
		COUT << obj.GetImePrezime() << " | " << obj.GetEmail() << " | " << obj.GetTelefon();
		return COUT;
	}
};
class Zivotinja {
	char* _oznaka;
	char* _vrsta;
	char* _opisStanja;
	int _redniBroj;
	Cuvar _cuvar;
	Kolekcija<StatusNastanjivanja, DatumVrijeme, 10> _statusi;
	vector<Pregled> _pregledi;
public:
	const char* GetOznaka() const { return _oznaka; }
	const char* GetVrsta() const { return _vrsta; }
	const char* GetOpisStanja() const { return _opisStanja; }
	int GetRedniBroj() const { return _redniBroj; }
	Cuvar& GetCuvar() { return _cuvar; }
	const Cuvar& GetCuvar() const { return _cuvar; }
	Kolekcija<StatusNastanjivanja, DatumVrijeme, 10>& GetStatusi() { return _statusi; }
	const Kolekcija<StatusNastanjivanja, DatumVrijeme, 10>& GetStatusi() const { return _statusi; }
	vector<Pregled>& GetPregledi() { return _pregledi; }
	const vector<Pregled>& GetPregledi() const { return _pregledi; }
	StatusNastanjivanja GetTrenutniStatus() const { return _statusi.GetElement1(_statusi.GetTrenutno() - 1); }
	friend ostream& operator<<(ostream& COUT, const Zivotinja& obj) {
		// ToString vraca:
		// oznaka | ime i prezime cuvara | vrsta | trenutni status
		COUT << obj.ToString();
		return COUT;
	}
	~Zivotinja() {
		delete[] _oznaka;
		delete[] _vrsta;
		delete[] _opisStanja;
		_oznaka = _vrsta = _opisStanja = nullptr;
	}
};
class Zoo {
	char* _naziv;
	vector<Zivotinja> _zivotinje;
public:
	Zoo(const char* naziv = "") {
		_naziv = AlocirajTekst(naziv);
	}
	const char* GetNaziv() const { return _naziv; }
	vector<Zivotinja>& GetZivotinje() { return _zivotinje; }
	const vector<Zivotinja>& GetZivotinje() const { return _zivotinje; }
	~Zoo() {
		delete[] _naziv; _naziv = nullptr;
	}
};
const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Klasa DatumVrijeme cuva sve svoje atribute kao pokazivace (int*). "
			"Objasnite zasto je u takvoj klasi neophodno eksplicitno implementirati konstruktor "
			"kopije i operator dodjele, te sta se dogodi ako se to izostavi.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasnite na koji nacin biste, koristeci fstream i standardne "
			"tokove, prebrojali koliko redova sadrzi tekstualni fajl.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
int main() {
	cout << PORUKA_TELEFON; cin.get();
	cout << PORUKA_TELEFON; cin.get();
	cout << PORUKA_ISPIT; cin.get(); system("cls");
	cout << GetOdgovorNaPrvoPitanje() << crt; cin.get();
	cout << GetOdgovorNaDrugoPitanje() << crt; cin.get();

	/*
	Funkcija GenerisiOznaku generise oznaku zivotinje u formatu:
	Z-BBB/VV-GGGG
	Potpis funkcije treba biti:
	string GenerisiOznaku(const char* vrsta, int redniBroj, int godina)
	Z -> fiksni prefiks,
	BBB -> redni broj popunjen nulama na slobodnim mjestima,
	VV -> inicijali naucnog (binomijalnog) naziva vrste,
	GGGG -> godina prijema u zoo.
	Naziv vrste se sastoji od dvije rijeci (rod i vrsta, npr. "Panthera tigris"),
	koriste se inicijali prve i druge rijeci.
	Redni broj mora biti u rasponu 1-999, a godina 2000-2099.
	Za neispravne podatke funkcija vraca "Z-000/XX-0000".
	*/
	if (GenerisiOznaku("Panthera tigris", 15, 2026) == "Z-015/PT-2026")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Loxodonta africana", 3, 2025) == "Z-003/LA-2025")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Aquila chrysaetos", 128, 2027) == "Z-128/AC-2027")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Zirafa", 15, 2026) == "Z-000/XX-0000" &&
		GenerisiOznaku("Panthera tigris", 0, 2026) == "Z-000/XX-0000" &&
		GenerisiOznaku("Panthera tigris", 1000, 2026) == "Z-000/XX-0000" &&
		GenerisiOznaku("Panthera tigris", 15, 1999) == "Z-000/XX-0000")
			cout << "Neispravni podaci za oznaku OK" << crt;

	/*
	Koristeci regex, funkcija ValidirajOznaku provjerava prethodno definisani format.
	Prefiks mora biti Z, redni broj mora imati tri cifre i ne moze biti 000,
	inicijali moraju biti velika slova, a godina je 2000-2099.
	Potpis funkcije treba biti:
	bool ValidirajOznaku(const string& oznaka)
	*/
	if (ValidirajOznaku("Z-015/PT-2026"))
		cout << "OZNAKA VALIDNA" << crt;
	if (!ValidirajOznaku("Z/015-PT-2026") &&
		!ValidirajOznaku("Z-15/PT-2026") &&
		!ValidirajOznaku("Z-015/Pt-2026") &&
		!ValidirajOznaku("Z-000/PT-2026") &&
		!ValidirajOznaku("2026-Z-015/PT"))
		cout << "OZNAKA NIJE VALIDNA" << crt;

	Kolekcija<int, string, 6> vrste;
	vrste.Dodaj(101, "Tigar");
	vrste.Dodaj(102, "Slon");
	vrste.Dodaj(101, "Tigar");
	vrste.Dodaj(103, "Zirafa");
	vrste.Dodaj(102, "Slon");
	cout << vrste << crt;

	/*
	UkloniDuplikate vraca novu kolekciju u kojoj je zadrzano samo prvo
	pojavljivanje svakog para. Par predstavlja duplikat iskljucivo ako su
	jednaki i prvi i drugi element. Originalna kolekcija ostaje nepromijenjena.
	*/
	Kolekcija<int, string, 6> bezDuplikata = vrste.UkloniDuplikate();
	cout << "Bez duplikata:" << crt << bezDuplikata;
	cout << "Original:" << crt << vrste;
	try {
		Kolekcija<int, string, 2> puna;
		puna.Dodaj(1, "Jedan");
		puna.Dodaj(2, "Dva");
		puna.Dodaj(3, "Tri");
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}
	Kolekcija<int, string, 6> kopijaVrsta = vrste;
	kopijaVrsta[0] = 999;
	Kolekcija<int, string, 6> dodijeljeneVrste;
	dodijeljeneVrste = vrste;
	dodijeljeneVrste.GetElement2(0) = "Izmijenjeno";
	cout << "Original:" << crt << vrste;
	cout << "Kopija:" << crt << kopijaVrsta;
	cout << "Dodijeljeni objekat:" << crt << dodijeljeneVrste;

	DatumVrijeme dosla(3, 5, 2026, 9, 0);
	DatumVrijeme karantin(3, 5, 2026, 10, 0);
	DatumVrijeme prilagodjavanje(10, 5, 2026, 9, 0);
	DatumVrijeme nastanjena(20, 5, 2026, 9, 0);

	/*
	ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM,
	ukljucujuci pocetne nule.
	*/
	cout << dosla.ToString() << crt; // 03.05.2026 09:00
	if (karantin > dosla)
		cout << "Vrijeme karantina je nakon prijema" << crt;
	DatumVrijeme kopijaDatuma(karantin);
	if (kopijaDatuma == karantin && !(dosla == karantin))
		cout << "Provjera vremena, OK." << crt;
	Cuvar amina("Amina Buric", "amina@zoo.ba", "061-111-222");
	Cuvar goran("Goran Skondric", "goran@zoo.ba", "062-222-333");
	Cuvar kopijaCuvara = amina;
	cout << kopijaCuvara << crt;

	/*
	Pregled sadrzi opis, ime veterinara, cijenu i trajanje u minutama.
	*/
	Pregled prijem("Prijemni pregled", "Dr. Haris Hadzic", 40, 30);
	Pregled cijepljenje("Vakcinacija", "Dr. Haris Hadzic", 65.5, 20);
	Pregled kontrola("Kontrolni pregled", "Dr. Maja Majic", 30, 15);

	/* ToString vraca podatke u formatu:
	opis | veterinar | cijena KM | trajanje min
	Vakcinacija | Dr. Haris Hadzic | 65.50 KM | 20 min
	*/
	cout << cijepljenje.ToString() << crt;

	/*
	Konstruktor Zivotinje generise oznaku na osnovu vrste, rednog broja i
	godine prijema, te evidentira pocetni status NOVOPRIDOSLA sa
	proslijedjenim vremenom.
	*/
	Zivotinja tigar("Panthera tigris", "Nemirna, ne jede jos", amina, 15, dosla);
	Zivotinja orao("Aquila chrysaetos", "Ozlijedjeno krilo", goran, 3, dosla);
	/*
	ToString vraca podatke u formatu:
	oznaka | ime i prezime cuvara | vrsta | trenutni status
	Z-015/PT-2026 | Amina Buric | Panthera tigris | NOVOPRIDOSLA
	*/
	cout << tigar.ToString() << crt;
	if (tigar.ToString() == "Z-015/PT-2026 | Amina Buric | Panthera tigris | NOVOPRIDOSLA")
		cout << "Zivotinja ToString OK" << crt;

	/*
	DodajStatus dodaje status samo ako je vrijeme vece od vremena posljednjeg
	statusa i ako status predstavlja neposredno sljedecu fazu.
	Dozvoljen slijed je: NOVOPRIDOSLA -> KARANTIN -> PRILAGODJAVANJE -> NASTANJENA
	Preskakanje ili ponavljanje statusa nije dozvoljeno. Nakon statusa
	NASTANJENA nisu dozvoljene nove promjene. Neuspjesan pokusaj vraca false
	bez izmjene kolekcije statusa.
	*/
	if (!tigar.DodajStatus(PRILAGODJAVANJE, karantin))
		cout << "Preskakanje statusa nije dozvoljeno" << crt;
	if (tigar.DodajStatus(KARANTIN, karantin))
		cout << "Status KARANTIN dodan" << crt;
	if (!tigar.DodajStatus(KARANTIN, prilagodjavanje))
		cout << "Ponavljanje statusa nije dozvoljeno" << crt;

	/*
	DodajPregled dodaje pregled samo dok je zivotinja u statusu
	KARANTIN ili PRILAGODJAVANJE, uz cijenu i trajanje vece od nule.
	Metoda vraca true ako je pregled dodan, u suprotnom false.
	UkupniTroskovi vraca zbir cijena svih pregleda, a UkupnoTrajanje
	zbir njihovih trajanja u minutama.
	*/
	if (tigar.DodajPregled(prijem))
		cout << "Pregled dodan" << crt;
	if (tigar.DodajStatus(PRILAGODJAVANJE, prilagodjavanje))
		cout << "Status PRILAGODJAVANJE dodan" << crt;
	tigar.DodajPregled(cijepljenje);
	tigar.DodajPregled(kontrola);
	cout << "Ukupni troskovi: " << tigar.UkupniTroskovi() << " KM" << crt;
	cout << "Ukupno trajanje: " << tigar.UkupnoTrajanje() << " min" << crt;
	Zivotinja kopijaTigra = tigar;
	cout << kopijaTigra << crt;
	Zoo sarajevskiZoo("Sarajevski Zoo");

	/*
	DodajZivotinju dodaje zivotinju u zoo. Nije dozvoljeno dodati dvije zivotinje
	sa istim rednim brojem ili istom oznakom. U slucaju duplikata metoda baca izuzetak.
	*/
	sarajevskiZoo.DodajZivotinju(tigar);
	sarajevskiZoo.DodajZivotinju(orao);
	try {
		sarajevskiZoo.DodajZivotinju(orao);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	/*
	PronadjiZivotinju vraca pokazivac na zivotinju sa proslijedjenom oznakom.
	Ako zivotinja nije pronadjena, metoda vraca nullptr.
	*/
	string oznakaTigra = tigar.GetOznaka();
	Zivotinja* pronadjena = sarajevskiZoo.PronadjiZivotinju(oznakaTigra);
	if (pronadjena != nullptr)
		cout << "Pronadjena zivotinja: " << pronadjena->GetOznaka() << crt;
	if (sarajevskiZoo.PronadjiZivotinju("Z-999/XX-2026") == nullptr)
		cout << "Zivotinja nije pronadjena" << crt;

	/*
	EvidentirajStatus pronalazi zivotinju i pokusava dodati status
	koristeci pravila metode DodajStatus.
	Izvjestaj o nastanjenju se salje u zasebnom threadu iskljucivo nakon
	uspjesnog evidentiranja statusa NASTANJENA. Za ostale statuse se ne
	salju obavijesti. Ako zivotinja ne postoji ili status nije dodan,
	metoda vraca false.
	Primjer sadrzaja izvjestaja:
	---------------------------------------------------------------------------------
	To: amina@zoo.ba
	From: uprava@zoo.ba
	Subject: Zivotinja nastanjena - izvjestaj
	Postovani Amina Buric,
	Zivotinja Z-015/PT-2026 (Panthera tigris) je uspjesno nastanjena.
	Ukupni troskovi pregleda: 95.50 KM
	Hvala na brizi.
	---------------------------------------------------------------------------------
	*/
	if (sarajevskiZoo.EvidentirajStatus(oznakaTigra, NASTANJENA, nastanjena))
		cout << "Zivotinja nastanjena i izvjestaj poslan" << crt;
	if (!sarajevskiZoo.EvidentirajStatus(oznakaTigra, NASTANJENA, DatumVrijeme(21, 5, 2026, 9, 0)))
		cout << "Ponavljanje zavrsnog statusa nije dozvoljeno" << crt;
	if (!sarajevskiZoo.EvidentirajStatus("NEPOSTOJECA", KARANTIN, karantin))
		cout << "Status nije evidentiran za nepostojecu zivotinju" << crt;

	/*
	IzdvojiNenastanjene vraca vector pokazivaca na sve zivotinje ciji
	trenutni status nije NASTANJENA.
	*/
	vector<Zivotinja*> nenastanjene = sarajevskiZoo.IzdvojiNenastanjene();
	for (auto zivotinja : nenastanjene)
		cout << zivotinja->GetOznaka() << " -> " << StatusNastanjivanjaNazivi[(int)zivotinja->GetTrenutniStatus()] << crt;

	/*
	IzracunajTroskove vraca zbir cijena svih pregleda evidentiranih na
	zivotinjama koje imaju status NASTANJENA. Pregledi na jos nenastanjenim
	zivotinjama ne ulaze u zbir.
	*/
	cout << "Ukupni troskovi nastanjivanja: " << sarajevskiZoo.IzracunajTroskove() << " KM" << crt;
	Zoo kopijaZoo = sarajevskiZoo;
	cout << kopijaZoo.GetNaziv() << " ima " << kopijaZoo.GetZivotinje().size() << " zivotinja" << crt;
	cin.get();
	return 0;
}