#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

string crt = "\n---------------------------------------------------------------------------------\n";

string PORUKA_TELEFON = crt +
"TELEFONE ISKLJUCITE I ODLOZITE U TORBU, DZEP ILI DRUGU LOKACIJU VAN DOHVATA.\n"
"CESTO SE NA TELEFONIMA (PRO)NALAZE PROGRAMSKI KODOVI KOJI MOGU BITI ISKORISTENI ZA\n"
"RJESAVANJE ISPITNOG ZADATKA, STO CE, U SLUCAJU PRONALASKA, BITI SANKCIONISANO." + crt;

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

enum TipAranzmana { ODMOR, EKSKURZIJA, AVANTURA, POSLOVNO };
const char* TipAranzmanaOznake[] = { "OD", "EK", "AV", "PO" };
const char* TipAranzmanaNazivi[] = { "ODMOR", "EKSKURZIJA", "AVANTURA", "POSLOVNO" };

string GenerisiOznaku(const char* imePrezime, int redniBroj, TipAranzmana tip);
bool ValidirajOznaku(const string& oznaka);

/*
Klasa Kolekcija cuva parove elemenata preko pokazivaca (T1*, T2*) u nizovima
fiksne velicine max. Kako klasa upravlja dinamicki alociranim resursima,
potrebno je implementirati i konstruktor kopije i operator dodjele (pravilo
trojke), tako da kopija i original imaju potpuno nezavisne resurse.
*/
template<class T1, class T2, int max>
class Kolekcija {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
public:
	Kolekcija() : _trenutno(0) {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	// konstruktor kopije - duboka kopija svih elemenata
	Kolekcija(const Kolekcija& obj) : _trenutno(0) {
		for (int i = 0; i < max; i++) {
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	// operator dodjele - duboka kopija, uz oslobadjanje postojecih resursa
	Kolekcija& operator=(const Kolekcija& obj) {
		return *this;
	}
	~Kolekcija() {
		for (int i = 0; i < _trenutno; i++) {
			delete _elementi1[i];
			delete _elementi2[i];
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
		_trenutno = 0;
	}
	int GetTrenutno() const { return _trenutno; }
	T1& GetElement1(int indeks) { return *_elementi1[indeks]; }
	T2& GetElement2(int indeks) { return *_elementi2[indeks]; }
	T1& operator[](int indeks) { return *_elementi1[indeks]; }
	void Dodaj(T1 e1, T2 e2) {
		_elementi1[_trenutno] = new T1(e1);
		_elementi2[_trenutno] = new T2(e2);
		_trenutno++;
	}
	// DodajNaPoziciju dodaje novi par na lokaciju/indeks definisanu prvim parametrom,
	// pomjera postojece pokazivace udesno i vraca trenutno stanje kolekcije (*this).
	// U slucaju popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak.
	Kolekcija& DodajNaPoziciju(int lokacija, T1 e1, T2 e2) {
		return *this;
	}
	// UkloniSaPozicije uklanja par sa proslijedjene pozicije/indeksa, preostale elemente
	// pomjera ulijevo i vraca uklonjeni par u obliku pair<T1, T2>.
	// Za poziciju izvan opsega baca se izuzetak.
	pair<T1, T2> UkloniSaPozicije(int pozicija) {
		return make_pair(T1(), T2());
	}
	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (int i = 0; i < obj._trenutno; i++)
			COUT << *obj._elementi1[i] << " " << *obj._elementi2[i] << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minute = 0) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minute = new int(minute);
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
	}
	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM, ukljucujuci pocetne nule
	string ToString() const {
		return "";
	}
	// operator> vraca true ako je *this hronoloski nakon obj
	bool operator>(const DatumVrijeme& obj) const {
		return false;
	}
	bool operator==(const DatumVrijeme& obj) const {
		return false;
	}
	friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj) {
		COUT << obj.ToString();
		return COUT;
	}
	~DatumVrijeme() {
		delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
		_dan = _mjesec = _godina = _sati = _minute = nullptr;
	}
};

class Aranzman {
	char* _oznaka;
	char* _naziv;
	TipAranzmana _tip;
	DatumVrijeme _pocetak;
	int _trajanjeDana;
public:
	Aranzman(const char* naziv, const char* imePrezimePutnika, int redniBroj,
		TipAranzmana tip, DatumVrijeme pocetak, int trajanjeDana)
		: _tip(tip), _pocetak(pocetak), _trajanjeDana(trajanjeDana) {
		_naziv = AlocirajTekst(naziv);
		_oznaka = AlocirajTekst(GenerisiOznaku(imePrezimePutnika, redniBroj, tip).c_str());
	}
	~Aranzman() {
		delete[] _oznaka; _oznaka = nullptr;
		delete[] _naziv; _naziv = nullptr;
	}
	const char* GetOznaka() const { return _oznaka; }
	const char* GetNaziv() const { return _naziv; }
	TipAranzmana GetTip() const { return _tip; }
	DatumVrijeme& GetPocetak() { return _pocetak; }
	int GetTrajanjeDana() const { return _trajanjeDana; }
	// ToString vraca podatke u formatu:
	// oznaka | naziv | tip | pocetak | broj dana
	// TA-EK-0042-AB | Obilazak Toskane | EKSKURZIJA | 15.07.2026 08:00 | 5
	string ToString() const {
		return "";
	}
	// ImaPreklapanjeSa vraca true ako se termini (od pocetka do pocetak+trajanjeDana)
	// dva aranzmana preklapaju. aranzman koji pocinje onog dana kada se prethodni
	// zavrsava nije preklapajuci.
	bool ImaPreklapanjeSa(Aranzman& drugi) {
		return false;
	}
};

class Putnik {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	vector<Aranzman> _aranzmani;
public:
	Putnik(const char* imePrezime = "") {
		_imePrezime = AlocirajTekst(imePrezime);
		_sifra = AlocirajTekst(GenerisiOznaku(imePrezime, _id, ODMOR).c_str());
		_id++;
	}
	~Putnik() {
		delete[] _sifra; _sifra = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	const char* GetSifra() const { return _sifra; }
	const char* GetImePrezime() const { return _imePrezime; }
	vector<Aranzman>& GetAranzmani() { return _aranzmani; }
	// PrijaviAranzman dodaje aranzman putniku ako se ne preklapa ni sa jednim vec
	// prijavljenim aranzmanom, i ako ukupan broj dana putovanja u kalendarskoj godini
	// (zbirno sa novim aranzmanom) ne prelazi 30 dana. metoda vraca true/false.
	bool PrijaviAranzman(Aranzman aranzman) {
		return false;
	}
	friend ostream& operator<<(ostream& COUT, Putnik& obj) {
		COUT << obj._imePrezime << " [" << obj._sifra << "]" << endl;
		for (auto& aranzman : obj._aranzmani)
			COUT << " - " << aranzman.ToString() << endl;
		return COUT;
	}
};
int Putnik::_id = 0;

class Agencija {
	char* _naziv;
	vector<Putnik> _putnici;
public:
	Agencija(const char* naziv = "") { _naziv = AlocirajTekst(naziv); }
	Agencija(const Agencija& obj) {
		_naziv = AlocirajTekst(obj._naziv);
		_putnici = obj._putnici;
	}
	~Agencija() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Putnik>& GetPutnici() { return _putnici; }
	// DodajPutnika onemogucava dodavanje putnika sa istom sifrom i baca izuzetak
	void DodajPutnika(Putnik putnik) {
		_putnici.push_back(putnik);
	}
	// PronadjiPutnika vraca pokazivac na putnika sa proslijedjenom sifrom.
	// Ako putnik nije pronadjen, metoda vraca nullptr.
	Putnik* PronadjiPutnika(const string& sifra) {
		return nullptr;
	}
	// PrijaviAranzmanZaPutnika pronalazi putnika po sifri i pokusava mu prijaviti
	// aranzman koristeci pravila metode PrijaviAranzman. Nakon uspjesne prijave,
	// u zasebnom thread-u, treba poslati obavijest putniku sa razmakom od 2 sekunde,
	// sadrzaja:
	//
	// To: <sifra putnika>@agencija.ba
	// From: info@agencija.ba
	// Subject: Potvrda rezervacije
	// Postovani <ime i prezime>,
	// Vasa rezervacija za aranzman <naziv aranzmana> (<tip>) je potvrdjena.
	// Polazak: <datum i vrijeme pocetka>
	// Prijatno putovanje!
	//
	// Ako putnik ne postoji ili prijava nije uspjela, obavijest se ne salje i
	// metoda vraca false.
	bool PrijaviAranzmanZaPutnika(const string& sifra, Aranzman aranzman) {
		return false;
	}
	// AktivniPutnici vraca pokazivace na putnike koji imaju najmanje onoliko
	// prijavljenih aranzmana koliko je definisano vrijednoscu parametra.
	vector<Putnik*> AktivniPutnici(int minBrojAranzmana) {
		vector<Putnik*> rezultat;
		return rezultat;
	}
	// StatistikaPoTipu vraca kolekciju koja za svaki tip aranzmana, redoslijedom
	// definisanim enumom TipAranzmana, sadrzi tip i ukupan broj prijavljenih
	// aranzmana tog tipa u agenciji (ukljucujuci tipove sa nula prijava).
	Kolekcija<TipAranzmana, int, 4> StatistikaPoTipu() {
		Kolekcija<TipAranzmana, int, 4> rezultat;
		return rezultat;
	}
	// operator() vraca sve putnike u agenciji koji imaju broj prijavljenih aranzmana
	// >= proslijedjenoj vrijednosti, sortirane opadajuce po broju aranzmana
	// (iskoristiti lambda izraz za sortiranje).
	vector<Putnik*> operator()(int minBrojAranzmana) {
		vector<Putnik*> rezultat;
		return rezultat;
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Objasnite pravilo trojke (rule of three) i kada je potrebno eksplicitno implementirati konstruktor kopije i operator dodjele \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Objasnite razliku izmedju agregacije i asocijacije uz konkretan primjer \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

int main() {
	cout << PORUKA_TELEFON;
	cin.get();
	cout << PORUKA_ISPIT;
	cin.get();
	system("cls");
	cout << GetOdgovorNaPrvoPitanje() << crt;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << crt;
	cin.get();

	/*
	Funkcija GenerisiOznaku generise oznaku u formatu: TA-TP-BBBB-IN
	TA -> fiksna oznaka agencije,
	TP -> oznaka tipa aranzmana: OD (odmor), EK (ekskurzija), AV (avantura), PO (poslovno),
	BBBB -> redni broj popunjen nulama na slobodnim mjestima (raspon 1-9999),
	IN -> inicijali prve i posljednje rijeci imena i prezimena.
	Za neispravne podatke funkcija vraca "TA-XX-0000-XX".
	*/
	if (GenerisiOznaku("Amina Buric", 42, EKSKURZIJA) == "TA-EK-0042-AB")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Goran Skondric", 7, ODMOR) == "TA-OD-0007-GS")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Ana Marija Kovac", 156, AVANTURA) == "TA-AV-0156-AK")
		cout << "Oznaka OK" << crt;
	if (GenerisiOznaku("Amina", 42, EKSKURZIJA) == "TA-XX-0000-XX" &&
		GenerisiOznaku("Amina Buric", 0, EKSKURZIJA) == "TA-XX-0000-XX" &&
		GenerisiOznaku("Amina Buric", 10000, EKSKURZIJA) == "TA-XX-0000-XX")
		cout << "Neispravni podaci za oznaku OK" << crt;

	// koristeci regex, funkcija ValidirajOznaku provjerava da li je oznaka
	// zapisana u prethodno definisanom formatu.
	if (ValidirajOznaku("TA-EK-0042-AB"))
		cout << "OZNAKA VALIDNA" << crt;
	if (!ValidirajOznaku("TA-XX-0042-AB") && !ValidirajOznaku("TA-EK-042-AB") &&
		!ValidirajOznaku("TA-EK-0042-Ab") && !ValidirajOznaku("TA-EK-0000-AB"))
		cout << "OZNAKA NIJE VALIDNA" << crt;

	Kolekcija<int, string, 5> brojevi;
	brojevi.Dodaj(10, "Deset");
	brojevi.Dodaj(20, "Dvadeset");
	brojevi.Dodaj(30, "Trideset");
	cout << brojevi << crt;

	Kolekcija<int, string, 5> prosireniBrojevi = brojevi.DodajNaPoziciju(1, 15, "Petnaest");
	cout << prosireniBrojevi << crt;

	pair<int, string> uklonjeni = prosireniBrojevi.UkloniSaPozicije(2);
	cout << "Uklonjeno: " << uklonjeni.first << " " << uklonjeni.second << crt;
	cout << "Preostali elementi:" << crt << prosireniBrojevi;

	try {
		prosireniBrojevi.UkloniSaPozicije(10);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	Kolekcija<int, string, 5> kopijaBrojeva = brojevi;
	kopijaBrojeva[0] = 100;
	Kolekcija<int, string, 5> dodijeljeniBrojevi;
	dodijeljeniBrojevi = brojevi;
	dodijeljeniBrojevi[1] = 200;
	cout << "Original:" << crt << brojevi;
	cout << "Kopija:" << crt << kopijaBrojeva;
	cout << "Dodijeljeni objekat:" << crt << dodijeljeniBrojevi;

	DatumVrijeme polazak1(15, 7, 2026, 8, 0);
	DatumVrijeme polazak2(18, 7, 2026, 9, 0);
	DatumVrijeme polazak3(1, 8, 2026, 7, 0);

	// ToString vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM, ukljucujuci pocetne nule
	cout << polazak1.ToString() << crt; // 15.07.2026 08:00

	if (polazak2 > polazak1)
		cout << "Polazak2 je hronoloski nakon polazak1" << crt;

	DatumVrijeme kopijaPolaska(polazak2);
	if (kopijaPolaska == polazak2 && !(polazak1 == polazak2))
		cout << "Provjera vremena, OK." << crt;

	Aranzman toskana("Obilazak Toskane", "Amina Buric", 42, EKSKURZIJA, polazak1, 5);
	Aranzman planine("Avantura na Alpama", "Goran Skondric", 7, AVANTURA, polazak2, 4);
	Aranzman konferencija("Poslovni summit", "Ana Marija Kovac", 156, POSLOVNO, polazak3, 3);

	// ToString vraca podatke u formatu:
	// oznaka | naziv | tip | pocetak | broj dana
	cout << toskana.ToString() << crt;
	if (toskana.ToString() == "TA-EK-0042-AB | Obilazak Toskane | EKSKURZIJA | 15.07.2026 08:00 | 5")
		cout << "Aranzman ToString OK" << crt;

	if (toskana.ImaPreklapanjeSa(planine))
		cout << "Termini se preklapaju" << crt;
	if (!toskana.ImaPreklapanjeSa(konferencija))
		cout << "Termini se ne preklapaju" << crt;

	Putnik amina("Amina Buric"), goran("Goran Skondric"), ana("Ana Marija Kovac");

	if (amina.PrijaviAranzman(toskana))
		cout << "Aranzman prijavljen" << crt;
	if (!amina.PrijaviAranzman(planine))
		cout << "Aranzman nije prijavljen - preklapanje termina" << crt;

	Agencija centarSarajevo("Centar Sarajevo"), poslovnicaMostar("Poslovnica Mostar");
	centarSarajevo.DodajPutnika(amina);
	centarSarajevo.DodajPutnika(goran);
	poslovnicaMostar.DodajPutnika(ana);

	try {
		centarSarajevo.DodajPutnika(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	string sifraGorana = goran.GetSifra();
	Putnik* pronadjen = centarSarajevo.PronadjiPutnika(sifraGorana);
	if (pronadjen != nullptr)
		cout << "Pronadjen putnik: " << pronadjen->GetImePrezime() << crt;
	if (centarSarajevo.PronadjiPutnika("TA-XX-9999-XX") == nullptr)
		cout << "Nepostojeci putnik nije pronadjen" << crt;

	if (centarSarajevo.PrijaviAranzmanZaPutnika(sifraGorana, planine))
		cout << "Prijava uspjesna, obavijest poslana" << crt;
	if (!centarSarajevo.PrijaviAranzmanZaPutnika("NEPOSTOJECA", konferencija))
		cout << "Obavijest nije poslana za nepostojeceg putnika" << crt;

	vector<Putnik*> aktivni = centarSarajevo.AktivniPutnici(1);
	for (auto putnik : aktivni)
		cout << putnik->GetImePrezime() << " ima " << putnik->GetAranzmani().size() << " aranzmana" << crt;

	Kolekcija<TipAranzmana, int, 4> statistika = centarSarajevo.StatistikaPoTipu();
	for (int i = 0; i < statistika.GetTrenutno(); i++)
		cout << TipAranzmanaNazivi[(int)statistika.GetElement1(i)] << " -> " << statistika.GetElement2(i) << crt;

	vector<Putnik*> topPutnici = centarSarajevo(1);
	for (auto putnik : topPutnici)
		cout << putnik->GetImePrezime() << crt;

	Agencija kopijaAgencije = centarSarajevo;
	cout << kopijaAgencije.GetNaziv() << " ima " << kopijaAgencije.GetPutnici().size() << " putnika" << crt;

	cin.get();
	return 0;
}