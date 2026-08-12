/*
	Napomena: postavka je nepotpuna po dizajnu - metode koje se pozivaju u
	main-u a nisu deklarisane u klasama treba samostalno dodati (deklaraciju
	i implementaciju), u skladu sa opisima datim u komentarima main-a.
*/

#include <iostream>
using namespace std;
const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. ATRIBUTI, NAZIVI METODA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI ONIMA KOJI SU KORISTENI "
"U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE POMOCNE "
"METODE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"4. IZUZETAK BACITE SAMO U METODAMA U KOJIMA JE TO NAZNACENO.\n"
"5. SVE METODE POZVANE U MAIN-U ZADATKA TREBAJU POSTOJATI. UKOLIKO NISTE ZADOVOLJNI IMPLEMENTACIJOM "
"POTREBNO JE DA IMPLEMENTIRATE BAREM TIJELO TIH METODA (METODA MOZE BITI PRAZNA), A AKO METODA TREBA "
"VRATITI NEKI PODATAK ONDA MOZETE VRATITI ZELJENU VRIJEDNOST ZAHTIJEVANOG TIPA.\n"
"6. NA KRAJU ISPITA SVOJE RJESENJE KOPIRAJTE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA npr. IB150051.docx)!\n"
"-------------------------------------------------------------------------------\n";
const char* crt = "\n---------------------------------------------------------------------------------\n";

char* AlocirajTekst(const char* tekst) {
	if (tekst == nullptr) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}

enum TipProstorije { OTVORENI_PROSTOR, SALA_ZA_SASTANKE, PRIVATNA_KANCELARIJA, TELEFONSKA_KABINA };
const char* TipProstorijeNazivi[] = {
"OTVORENI PROSTOR", "SALA ZA SASTANKE", "PRIVATNA KANCELARIJA", "TELEFONSKA KABINA" };

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
	~Kolekcija() {
		delete[] _elementi1; delete[] _elementi2;
		_elementi1 = nullptr; _elementi2 = nullptr;
	}
	int GetTrenutno() const { return _trenutno; }
	T1& GetElement1(int i) { return _elementi1[i]; }
	T2& GetElement2(int i) { return _elementi2[i]; }
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	// Dodaj, IzdvojiIznadVrijednosti, konstruktor kopije i operator dodjele
	// nisu dati - dodati po potrebi (paziti na duboko kopiranje niza pokazivaca).
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minute = 0) {
		_dan = new int(dan); _mjesec = new int(mjesec); _godina = new int(godina);
		_sati = new int(sati); _minute = new int(minute);
	}
	~DatumVrijeme() {
		delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
		_dan = _mjesec = _godina = _sati = _minute = nullptr;
	}
	int GetGodina() const { return *_godina; }
	int GetMjesec() const { return *_mjesec; }
	int GetDan() const { return *_dan; }
	int GetSati() const { return *_sati; }
	int GetMinute() const { return *_minute; }
	// ToString treba vratiti datum i vrijeme u formatu DD.MM.GGGG HH:MM (sa pocetnim nulama).
	// Iskoristiti je i u okviru operator<<.
	string ToString() const;
};

/*
Rezervacija je apstraktna bazna klasa. Cuva pocetak termina i trajanje u
minutama. Izvedene klase su PojedinacnaRezervacija i GrupnaRezervacija
(koja dodatno cuva broj ucesnika). Info() je cisto virtuelna metoda -
svaka izvedena klasa je implementira na svoj nacin.
*/
class Rezervacija {
protected:
	DatumVrijeme _pocetak;
	int _trajanjeMinuta;
public:
	Rezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta)
		: _pocetak(pocetak), _trajanjeMinuta(trajanjeMinuta) {
	}
	virtual ~Rezervacija() {}
	virtual string Info() const = 0;
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeMinuta() const { return _trajanjeMinuta; }
};
class PojedinacnaRezervacija : public Rezervacija {
public:
	PojedinacnaRezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta)
		: Rezervacija(pocetak, trajanjeMinuta) {
	}
	// Info() nije preklopljena - dodati override koji vraca npr:
	// "03.05.2026 09:00 POJEDINACNA REZERVACIJA (60 min)"
};
class GrupnaRezervacija : public Rezervacija {
	int _brojUcesnika;
public:
	GrupnaRezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta, int brojUcesnika)
		: Rezervacija(pocetak, trajanjeMinuta), _brojUcesnika(brojUcesnika) {
	}
	// Info() nije preklopljena - dodati override koji vraca npr:
	// "03.05.2026 11:00 GRUPNA REZERVACIJA ZA 6 OSOBA (90 min)"
	// Po potrebi dodati i getter za _brojUcesnika (koristi se prilikom provjere kapaciteta).
};

class Clan {
	static int _id;
	char* _sifra; // NIJE inicijalizirana u konstruktoru - generisati pomocu GenerisiSifruClana (opisano u main-u)
	char* _imePrezime;
	char* _email;
	vector<Rezervacija*> _rezervacije;
public:
	Clan(string imePrezime, string email) {
		_imePrezime = AlocirajTekst(imePrezime.c_str());
		_email = AlocirajTekst(email.c_str());
	}
	~Clan() {
		delete[] _imePrezime; delete[] _email; delete[] _sifra;
		for (auto* r : _rezervacije) delete r;
		_rezervacije.clear();
	}
	const char* GetImePrezime() const { return _imePrezime; }
	const char* GetEmail() const { return _email; }
	const char* GetSifra() const { return _sifra; }
	// operator<< nije dat - format prikaza (clan + njegove rezervacije preko Info())
	// osmisliti samostalno.
};
int Clan::_id = 1;

class Prostorija {
	char* _oznaka; // NIJE inicijalizirana u konstruktoru - generisati pomocu GenerisiOznakuProstorije (opisano u main-u)
	char* _naziv;
	TipProstorije _tip;
	int _kapacitet;
	double _cijenaPoSatu;
	vector<Rezervacija*> _rezervacije; // ne-vlasnicki pokazivaci, vlasnik je Clan
public:
	Prostorija(const char* naziv, TipProstorije tip, int kapacitet, double cijenaPoSatu, int sprat) {
		_naziv = AlocirajTekst(naziv);
		_tip = tip;
		_kapacitet = kapacitet;
		_cijenaPoSatu = cijenaPoSatu;
	}
	~Prostorija() {
		delete[] _oznaka; delete[] _naziv;
		_oznaka = nullptr; _naziv = nullptr;
	}
	const char* GetOznaka() const { return _oznaka; }
	const char* GetNaziv() const { return _naziv; }
	TipProstorije GetTip() const { return _tip; }
	int GetKapacitet() const { return _kapacitet; }
	double GetCijenaPoSatu() const { return _cijenaPoSatu; }
	// operator<<, DodajRezervaciju, UkupanPrihod, UkupnoZauzetihSati nisu dati - dodati po potrebi.
};

class UrbanHub {
	char* _naziv;
	vector<Clan> _clanovi;
	vector<Prostorija> _prostorije;
public:
	UrbanHub(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	~UrbanHub() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Clan>& GetClanovi() { return _clanovi; }
	vector<Prostorija>& GetProstorije() { return _prostorije; }
	// DodajClana, DodajProstoriju, PronadjiClana, PronadjiProstoriju, RezervisiProstoriju,
	// IzdvojiPreopterecene, UkupanPrihodHuba nisu dati - dodati po potrebi.
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Objasnite razliku izmedju agregacije i kompozicije, uz kratak primjer.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Sta je cisto virtuelna metoda i cemu sluzi apstraktna klasa? \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

int main() {
	cout << PORUKA;
	cin.get();
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	/*
	Funkcija GenerisiOznakuProstorije generise oznaku prostorije u formatu:
	CW-KKK/VV-S
	CW -> fiksni prefiks,
	KKK -> kapacitet popunjen nulama na slobodnim mjestima (1-500),
	VV -> inicijali naziva prostorije (naziv se sastoji od dvije rijeci, npr. "Sala Feniks"),
	S -> broj sprata (0-9).
	Potpis funkcije treba biti:
	string GenerisiOznakuProstorije(const char* naziv, int kapacitet, int sprat)
	Za neispravne podatke funkcija vraca "CW-000/XX-X". Iskoristiti je za inicijalizaciju
	atributa _oznaka u konstruktoru klase Prostorija.
	*/
	if (GenerisiOznakuProstorije("Sala Feniks", 12, 3) == "CW-012/SF-3")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznakuProstorije("Kabina Nova", 1, 0) == "CW-001/KN-0")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznakuProstorije("Prostor", 20, 2) == "CW-000/XX-X" &&
		GenerisiOznakuProstorije("Sala Feniks", 0, 3) == "CW-000/XX-X" &&
		GenerisiOznakuProstorije("Sala Feniks", 12, 10) == "CW-000/XX-X")
		cout << "Neispravni podaci za oznaku OK" << crt;

	/*
	Koristeci regex, funkcija ValidirajOznakuProstorije provjerava prethodno
	definisani format. Kapacitet ne moze biti 000, inicijali moraju biti
	velika slova, sprat jedna cifra.
	Potpis funkcije treba biti:
	bool ValidirajOznakuProstorije(const string& oznaka)
	*/
	if (ValidirajOznakuProstorije("CW-012/SF-3"))
		cout << "OZNAKA VALIDNA" << crt;
	if (!ValidirajOznakuProstorije("CW-000/SF-3") &&
		!ValidirajOznakuProstorije("CW-12/SF-3") &&
		!ValidirajOznakuProstorije("CW-012/sf-3"))
		cout << "OZNAKA NIJE VALIDNA" << crt;

	Kolekcija<string, double, 6> cjenovnik;
	cjenovnik.Dodaj("Otvoreni prostor", 5.0);
	cjenovnik.Dodaj("Sala za sastanke", 15.0);
	cjenovnik.Dodaj("Privatna kancelarija", 25.0);
	cjenovnik.Dodaj("Telefonska kabina", 8.0);
	cout << cjenovnik << crt;

	/*
	IzdvojiIznadVrijednosti treba vratiti novu kolekciju koja sadrzi samo one
	parove kod kojih je drugi element (cijena) veci ili jednak proslijedjenoj
	granici. Originalna kolekcija ostaje nepromijenjena.
	*/
	Kolekcija<string, double, 6> skuplje = cjenovnik.IzdvojiIznadVrijednosti(10.0);
	cout << "Iznad 10 KM/h:" << crt << skuplje;

	DatumVrijeme sastanak1(3, 5, 2026, 9, 0);
	DatumVrijeme sastanak2(3, 5, 2026, 9, 30); // preklapa se sa sastanak1
	DatumVrijeme sastanak3(3, 5, 2026, 11, 0);

	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM
	cout << sastanak1.ToString() << crt;

	Prostorija feniks("Sala Feniks", SALA_ZA_SASTANKE, 8, 15.0, 3);
	Prostorija kabina("Kabina Nova", TELEFONSKA_KABINA, 1, 8.0, 0);
	cout << feniks << crt;

	/*
	GenerisiSifruClana generise sifru clana u formatu CL-GG-BBB gdje su GG
	posljednje dvije cifre tekuce godine, a BBB troznamenkasti redni broj
	clana zasnovan na statickom brojacu _id. Iskoristiti je za inicijalizaciju
	atributa _sifra u konstruktoru klase Clan.
	*/
	UrbanHub hub("Urban Hub Sarajevo");
	/*
	DodajProstoriju dodaje prostoriju u hub. Ne dozvoliti dvije prostorije sa
	istom oznakom - baciti izuzetak.
	*/
	hub.DodajProstoriju(feniks);
	hub.DodajProstoriju(kabina);
	try {
		hub.DodajProstoriju(feniks); // ista oznaka, ocekivan izuzetak
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	/*
	DodajClana dodaje clana u hub. Ne dozvoliti dva clana sa istom sifrom -
	baciti izuzetak.
	*/
	Clan amina("Amina Buric", "amina@mail.com");
	Clan goran("Goran Skondric", "goran@mail.com");
	hub.DodajClana(amina);
	hub.DodajClana(goran);

	string oznakaFeniksa = hub.GetProstorije()[0].GetOznaka();

	/*
	RezervisiProstoriju pronalazi clana (po sifri) i prostoriju (po oznaci) i
	pokusava dodati rezervaciju u prostoriju. Rezervacija se prihvata samo
	ako se termin ne preklapa ni sa jednim vec postojecim terminom u istoj
	prostoriji, i (za grupne rezervacije) ako broj ucesnika ne prelazi
	kapacitet prostorije. Ako je rezervacija prihvacena, evidentira se i kod
	clana. Za grupne rezervacije sa 5 ili vise ucesnika, u zasebnom threadu
	se salje email clanu o potvrdi rezervacije, sadrzaja:
	---------------------------------------------------------------------------------
	To: goran@mail.com
	From: rezervacije@urbanhub.ba
	Subject: Potvrda grupne rezervacije
	Postovani Goran Skondric,
	Vasa grupna rezervacija za 6 osoba u prostoriji CW-008/SF-3 je potvrdjena.
	Hvala na povjerenju.
	---------------------------------------------------------------------------------
	Metoda vraca true ako je rezervacija uspjesno kreirana, u suprotnom false.
	*/
	hub.RezervisiProstoriju(amina.GetSifra(), oznakaFeniksa, new PojedinacnaRezervacija(sastanak1, 60));
	if (!hub.RezervisiProstoriju(amina.GetSifra(), oznakaFeniksa, new PojedinacnaRezervacija(sastanak2, 30)))
		cout << "Preklapajuca rezervacija odbijena" << crt;

	if (!hub.RezervisiProstoriju(goran.GetSifra(), oznakaFeniksa, new GrupnaRezervacija(sastanak3, 60, 12)))
		cout << "Grupna rezervacija odbijena (broj ucesnika premasuje kapacitet)" << crt;

	if (hub.RezervisiProstoriju(goran.GetSifra(), oznakaFeniksa, new GrupnaRezervacija(sastanak3, 90, 6)))
		cout << "Grupna rezervacija prihvacena i email poslan" << crt;

	// UkupanPrihod vraca zbir (cijenaPoSatu * trajanje_u_satima) za sve rezervacije prostorije.
	cout << hub.GetProstorije()[0].GetOznaka() << " prihod: " << hub.GetProstorije()[0].UkupanPrihod() << " KM" << crt;
	// UkupanPrihodHuba vraca zbir prihoda svih prostorija u hubu.
	cout << "Ukupan prihod huba: " << hub.UkupanPrihodHuba() << " KM" << crt;

	/*
	IzdvojiPreopterecene vraca pokazivace na sve prostorije cija je ukupna
	zauzetost (u satima, preko svih rezervacija) veca ili jednaka
	proslijedjenoj vrijednosti minSati.
	*/
	vector<Prostorija*> preopterecene = hub.IzdvojiPreopterecene(2);
	for (auto* p : preopterecene)
		cout << p->GetOznaka() << " -> " << p->UkupnoZauzetihSati() << "h zauzetosti" << crt;

	/*
	Funkcija UcitajClanove ucitava clanove iz fajla cije ime se proslijedjuje
	kao parametar. Svaka linija u fajlu treba biti u formatu
	"ime i prezime|email". Funkcija za svaki red kreira novog clana (ukoliko
	vec ne postoji clan sa istim imenom i prezimenom) i dodaje ga u
	proslijedjeni vektor. Funkcija vraca true ako je u vektor ucitan
	najmanje jedan podatak, a false u suprotnom.
	Primjer sadrzaja fajla:
	Amina Buric|amina@mail.com
	Goran Skondric|goran@mail.com
	*/
	string nazivFajla = "clanovi.txt";
	vector<Clan> ucitaniClanovi;
	if (UcitajClanove(nazivFajla, ucitaniClanovi))
		cout << "Ucitavanje uspjesno, ucitano " << ucitaniClanovi.size() << " clanova" << crt;
	else
		cout << "Ucitavanje neuspjesno (fajl ne postoji ili je prazan)" << crt;

	cin.get();
	return 0;
}