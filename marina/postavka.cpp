#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <thread>
#include <functional>
#include <algorithm>
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

enum TipPlovila { JEDRILICA, MOTORNI_CAMAC, KATAMARAN, JAHTA };
const char* TipPlovilaNazivi[] = {
"JEDRILICA", "MOTORNI CAMAC", "KATAMARAN", "JAHTA" };

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
	// Dodaj, IzdvojiUOpsegu, konstruktor kopije i operator dodjele
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
Najam je apstraktna bazna klasa. Cuva pocetak termina i trajanje u satima.
Izvedene klase su IndividualniNajam i GrupniNajam (koja dodatno cuva broj
putnika). Info() je cisto virtuelna metoda - svaka izvedena klasa je
implementira na svoj nacin.
*/
class Najam {
protected:
	DatumVrijeme _pocetak;
	int _trajanjeSati;
public:
	Najam(const DatumVrijeme& pocetak, int trajanjeSati)
		: _pocetak(pocetak), _trajanjeSati(trajanjeSati) {
	}
	virtual ~Najam() {}
	virtual string Info() const = 0;
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeSati() const { return _trajanjeSati; }
};
class IndividualniNajam : public Najam {
public:
	IndividualniNajam(const DatumVrijeme& pocetak, int trajanjeSati)
		: Najam(pocetak, trajanjeSati) {
	}
	// Info() nije preklopljena - dodati override koji vraca npr:
	// "03.05.2026 09:00 INDIVIDUALNI NAJAM (3h)"
};
class GrupniNajam : public Najam {
	int _brojPutnika;
public:
	GrupniNajam(const DatumVrijeme& pocetak, int trajanjeSati, int brojPutnika)
		: Najam(pocetak, trajanjeSati), _brojPutnika(brojPutnika) {
	}
	// Info() nije preklopljena - dodati override koji vraca npr:
	// "03.05.2026 11:00 GRUPNI NAJAM ZA 6 OSOBA (2h)"
	// Po potrebi dodati i getter za _brojPutnika (koristi se prilikom provjere kapaciteta).
};

class Klijent {
	static int _id;
	char* _sifra; // NIJE inicijalizirana u konstruktoru - generisati pomocu GenerisiSifruKlijenta (opisano u main-u)
	char* _imePrezime;
	char* _brojTelefona;
	vector<Najam*> _najmovi;
public:
	Klijent(string imePrezime, string brojTelefona) {
		_imePrezime = AlocirajTekst(imePrezime.c_str());
		_brojTelefona = AlocirajTekst(brojTelefona.c_str());
	}
	~Klijent() {
		delete[] _imePrezime; delete[] _brojTelefona; delete[] _sifra;
		for (auto* n : _najmovi) delete n;
		_najmovi.clear();
	}
	const char* GetImePrezime() const { return _imePrezime; }
	const char* GetBrojTelefona() const { return _brojTelefona; }
	const char* GetSifra() const { return _sifra; }
	// operator<< nije dat - format prikaza (klijent + njegovi najmovi preko Info())
	// osmisliti samostalno.
};
int Klijent::_id = 1;

class Plovilo {
	char* _oznaka; // NIJE inicijalizirana u konstruktoru - generisati pomocu GenerisiOznakuPlovila
	char* _naziv;
	TipPlovila _tip;
	int _kapacitet;
	double _cijenaPoSatu;
	vector<Najam*> _najmovi; // ne-vlasnicki pokazivaci, vlasnik je Klijent
public:
	Plovilo(const char* naziv, TipPlovila tip, int kapacitet, double cijenaPoSatu, int godinaProizvodnje) {
		_naziv = AlocirajTekst(naziv);
		_tip = tip;
		_kapacitet = kapacitet;
		_cijenaPoSatu = cijenaPoSatu;
	}
	~Plovilo() {
		delete[] _oznaka; delete[] _naziv;
		_oznaka = nullptr; _naziv = nullptr;
	}
	const char* GetOznaka() const { return _oznaka; }
	const char* GetNaziv() const { return _naziv; }
	TipPlovila GetTip() const { return _tip; }
	int GetKapacitet() const { return _kapacitet; }
	double GetCijenaPoSatu() const { return _cijenaPoSatu; }
	// operator<<, DodajNajam, UkupanPrihod, UkupnoZauzetihSati nisu dati - dodati po potrebi.
};

class Marina {
	char* _naziv;
	vector<Klijent> _klijenti;
	vector<Plovilo> _plovila;
public:
	Marina(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	~Marina() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Klijent>& GetKlijenti() { return _klijenti; }
	vector<Plovilo>& GetPlovila() { return _plovila; }
	// DodajKlijenta, DodajPlovilo, PronadjiKlijenta, PronadjiPlovilo, NajmiPlovilo,
	// IzdvojiPreopterecena, UkupanPrihodMarine, IzdvojiPoUslovu nisu dati - dodati po potrebi.
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Objasnite razliku izmedju prijateljske (friend) funkcije i clanske funkcije klase, uz kratak primjer.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Sta je preopterecenje operatora (operator overloading) i kada je korisno? \n";
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
	Funkcija GenerisiOznakuPlovila generise oznaku plovila u formatu:
	MR-KKK/VV-G
	MR -> fiksni prefiks,
	KKK -> kapacitet popunjen nulama na slobodnim mjestima (1-200),
	VV -> inicijali naziva plovila (naziv se sastoji od dvije rijeci, npr. "Jedrilica Vjetar"),
	G -> posljednja cifra godine proizvodnje.
	Potpis funkcije treba biti:
	string GenerisiOznakuPlovila(const char* naziv, int kapacitet, int godinaProizvodnje)
	Za neispravne podatke funkcija vraca "MR-000/XX-X". Iskoristiti je za inicijalizaciju
	atributa _oznaka u konstruktoru klase Plovilo.
	*/
	if (GenerisiOznakuPlovila("Jedrilica Vjetar", 8, 2023) == "MR-008/JV-3")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznakuPlovila("Katamaran Plava", 25, 2019) == "MR-025/KP-9")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznakuPlovila("Plovilo", 20, 2020) == "MR-000/XX-X" &&
		GenerisiOznakuPlovila("Jedrilica Vjetar", 0, 2020) == "MR-000/XX-X" &&
		GenerisiOznakuPlovila("Jedrilica Vjetar", 8, 2020) != "MR-000/XX-X")
		cout << "Neispravni podaci za oznaku OK" << crt;

	/*
	Koristeci regex, funkcija ValidirajOznakuPlovila provjerava prethodno
	definisani format. Kapacitet ne moze biti 000, inicijali moraju biti
	velika slova, cifra godine jedna znamenka.
	Potpis funkcije treba biti:
	bool ValidirajOznakuPlovila(const string& oznaka)
	*/
	if (ValidirajOznakuPlovila("MR-008/JV-3"))
		cout << "OZNAKA VALIDNA" << crt;
	if (!ValidirajOznakuPlovila("MR-000/JV-3") &&
		!ValidirajOznakuPlovila("MR-08/JV-3") &&
		!ValidirajOznakuPlovila("MR-008/jv-3"))
		cout << "OZNAKA NIJE VALIDNA" << crt;

	Kolekcija<string, double, 6> cjenovnik;
	cjenovnik.Dodaj("Jedrilica", 12.0);
	cjenovnik.Dodaj("Motorni camac", 18.0);
	cjenovnik.Dodaj("Katamaran", 30.0);
	cjenovnik.Dodaj("Jahta", 45.0);
	cout << cjenovnik << crt;

	/*
	IzdvojiUOpsegu treba vratiti novu kolekciju koja sadrzi samo one parove
	kod kojih je drugi element (cijena po satu) veci ili jednak donjoj
	granici i manji ili jednak gornjoj granici. Originalna kolekcija ostaje
	nepromijenjena.
	*/
	Kolekcija<string, double, 6> srednjaKlasa = cjenovnik.IzdvojiUOpsegu(15.0, 35.0);
	cout << "Cijena izmedju 15 i 35 KM/h:" << crt << srednjaKlasa;

	DatumVrijeme voznja1(3, 7, 2026, 9, 0);
	DatumVrijeme voznja2(3, 7, 2026, 9, 30); // preklapa se sa voznja1
	DatumVrijeme voznja3(3, 7, 2026, 11, 0);

	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM
	cout << voznja1.ToString() << crt;

	Plovilo vjetar("Jedrilica Vjetar", JEDRILICA, 8, 12.0, 2023);
	Plovilo nova("Camac Nova", MOTORNI_CAMAC, 1, 18.0, 2020);
	cout << vjetar << crt;

	/*
	GenerisiSifruKlijenta generise sifru klijenta u formatu KL-GG-BBB gdje su
	GG posljednje dvije cifre tekuce godine, a BBB troznamenkasti redni broj
	klijenta zasnovan na statickom brojacu _id. Iskoristiti je za
	inicijalizaciju atributa _sifra u konstruktoru klase Klijent.
	*/
	Marina marina("Marina Riviera");
	/*
	DodajPlovilo dodaje plovilo u marinu. Ne dozvoliti dva plovila sa istom
	oznakom - baciti izuzetak.
	*/
	marina.DodajPlovilo(vjetar);
	marina.DodajPlovilo(nova);
	try {
		marina.DodajPlovilo(vjetar); // ista oznaka, ocekivan izuzetak
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	/*
	DodajKlijenta dodaje klijenta u marinu. Ne dozvoliti dva klijenta sa
	istom sifrom - baciti izuzetak.
	*/
	Klijent ana("Ana Kovac", "061111222");
	Klijent marko("Marko Ilic", "062333444");
	marina.DodajKlijenta(ana);
	marina.DodajKlijenta(marko);

	string oznakaVjetra = marina.GetPlovila()[0].GetOznaka();

	/*
	NajmiPlovilo pronalazi klijenta (po sifri) i plovilo (po oznaci) i
	pokusava dodati najam u plovilo. Najam se prihvata samo ako se termin ne
	preklapa ni sa jednim vec postojecim terminom na istom plovilu, i (za
	grupne najmove) ako broj putnika ne prelazi kapacitet plovila. Ako je
	najam prihvacen, evidentira se i kod klijenta. Za grupne najmove sa 5 ili
	vise putnika, u zasebnom threadu se salje SMS obavijest klijentu o
	potvrdi najma, sadrzaja:
	---------------------------------------------------------------------------------
	Broj: 062333444
	Od: Marina Riviera
	Poruka: Postovani Marko Ilic, Vasa grupna voznja za 6 osoba na plovilu
	MR-008/JV-3 je potvrdjena. Hvala na povjerenju.
	---------------------------------------------------------------------------------
	Metoda vraca true ako je najam uspjesno kreiran, u suprotnom false.
	*/
	marina.NajmiPlovilo(ana.GetSifra(), oznakaVjetra, new IndividualniNajam(voznja1, 3));
	if (!marina.NajmiPlovilo(ana.GetSifra(), oznakaVjetra, new IndividualniNajam(voznja2, 2)))
		cout << "Preklapajuci najam odbijen" << crt;

	if (!marina.NajmiPlovilo(marko.GetSifra(), oznakaVjetra, new GrupniNajam(voznja3, 2, 12)))
		cout << "Grupni najam odbijen (broj putnika premasuje kapacitet)" << crt;

	if (marina.NajmiPlovilo(marko.GetSifra(), oznakaVjetra, new GrupniNajam(voznja3, 3, 6)))
		cout << "Grupni najam prihvacen i SMS poslan" << crt;

	// UkupanPrihod vraca zbir (cijenaPoSatu * trajanjeSati) za sve najmove plovila.
	cout << marina.GetPlovila()[0].GetOznaka() << " prihod: " << marina.GetPlovila()[0].UkupanPrihod() << " KM" << crt;
	// UkupanPrihodMarine vraca zbir prihoda svih plovila u marini.
	cout << "Ukupan prihod marine: " << marina.UkupanPrihodMarine() << " KM" << crt;

	/*
	IzdvojiPreopterecena vraca pokazivace na sva plovila cija je ukupna
	zauzetost (u satima, preko svih najmova) veca ili jednaka proslijedjenoj
	vrijednosti minSati.
	*/
	vector<Plovilo*> preopterecena = marina.IzdvojiPreopterecena(3);
	for (auto* p : preopterecena)
		cout << p->GetOznaka() << " -> " << p->UkupnoZauzetihSati() << "h zauzetosti" << crt;

	/*
	IzdvojiPoUslovu je "moderniji" nacin filtriranja - prima std::function
	predikat (npr. lambda izraz) i vraca pokazivace na sva plovila koja
	zadovoljavaju uslov. Potpis metode treba biti:
	vector<Plovilo*> IzdvojiPoUslovu(function<bool(const Plovilo&)> uslov)
	*/
	vector<Plovilo*> skupaPlovila = marina.IzdvojiPoUslovu([](const Plovilo& p) {
		return p.GetCijenaPoSatu() >= 15.0;
		});
	cout << "Plovila skuplja od 15 KM/h:" << crt;
	for (auto* p : skupaPlovila)
		cout << p->GetNaziv() << " (" << p->GetCijenaPoSatu() << " KM/h)" << crt;

	// Sortiranje plovila po kapacitetu opadajuce, koristeci std::sort i lambda komparator.
	sort(marina.GetPlovila().begin(), marina.GetPlovila().end(),
		[](const Plovilo& a, const Plovilo& b) { return a.GetKapacitet() > b.GetKapacitet(); });
	cout << "Plovila sortirana po kapacitetu:" << crt;
	for (auto& p : marina.GetPlovila())
		cout << p.GetNaziv() << " - kapacitet " << p.GetKapacitet() << crt;

	/*
	Funkcija UcitajKlijente ucitava klijente iz fajla cije ime se
	proslijedjuje kao parametar. Svaka linija u fajlu treba biti u formatu
	"ime i prezime|broj telefona". Funkcija za svaki red kreira novog
	klijenta (ukoliko vec ne postoji klijent sa istim imenom i prezimenom) i
	dodaje ga u proslijedjeni vektor. Funkcija vraca true ako je u vektor
	ucitan najmanje jedan podatak, a false u suprotnom.
	Primjer sadrzaja fajla:
	Ana Kovac|061111222
	Marko Ilic|062333444
	*/
	string nazivFajla = "klijenti.txt";
	vector<Klijent> ucitaniKlijenti;
	if (UcitajKlijente(nazivFajla, ucitaniKlijenti))
		cout << "Ucitavanje uspjesno, ucitano " << ucitaniKlijenti.size() << " klijenata" << crt;
	else
		cout << "Ucitavanje neuspjesno (fajl ne postoji ili je prazan)" << crt;

	cin.get();
	return 0;
}