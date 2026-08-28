#include <iostream>
#include <vector>
#include <string>
#include <regex>
using namespace std;

string crt = "\n------------------------------------------------------ -------------------------- - \n";

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
	if (!tekst) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}

string GenerisiSifru(const char* imePrezime, int redniBroj);

bool ValidirajSifru(const string& sifra);

enum TipVozila { EKONOMSKA, SUV, LIMUZINA, KOMBI };

const char* TipVozilaNazivi[] = { "EKONOMSKA", "SUV", "LIMUZINA", "KOMBI" };

template<class T1, class T2, int max>
class Kolekcija {
	T1 _prvi[max];
	T2 _drugi[max];
	int* _trenutno;
public:
	Kolekcija() { _trenutno = new int(0); }
	~Kolekcija() { delete _trenutno; _trenutno = nullptr; }
	int GetTrenutno() const { return *_trenutno; }
	T1& GetPrvi(int indeks) { return _prvi[indeks]; }
	T2& GetDrugi(int indeks) { return _drugi[indeks]; }
	T1& operator[](int indeks) { return _prvi[indeks]; }
	void Dodaj(T1 e1, T2 e2) {
		_prvi[*_trenutno] = e1;
		_drugi[*_trenutno] = e2;
		(*_trenutno)++;
	}
	// DodajNaPoziciju dodaje novi par na lokaciju definisanu prvim parametrom, pomjera
	// postojece elemente udesno i vraca trenutno stanje kolekcije (*this). U slucaju
	// popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak.
	Kolekcija& DodajNaPoziciju(int lokacija, T1 e1, T2 e2) {
		return *this;
	}
	// UkloniRaspon od lokacije definisane prvim parametrom uklanja broj elemenata definisan
	// drugim parametrom, a vraca pokazivac na novu kolekciju koja sadrzi uklonjene elemente.
	// Pozivalac je odgovoran za dealokaciju vracene kolekcije. Za neispravan raspon baciti izuzetak.
	Kolekcija* UkloniRaspon(int odLokacije, int brojElemenata) {
		Kolekcija* uklonjeni = new Kolekcija();
		return uklonjeni;
	}
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetPrvi(i) << " " << obj.GetDrugi(i) << endl;
		return COUT;
	}
};

class DatumVrijeme {
	int* _godina, * _mjesec, * _dan, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int
		sati = 0, int minute = 0) {
		_godina = new int(godina);
		_mjesec = new int(mjesec);
		_dan = new int(dan);
		_sati = new int(sati);
		_minute = new int(minute);
	}
	~DatumVrijeme() {
		delete _godina; delete _mjesec; delete _dan;
		delete _sati; delete _minute;
	}
	friend ostream& operator<<(ostream& COUT, DatumVrijeme& obj) {
		COUT << *obj._dan << "." << *obj._mjesec << "." <<
			*obj._godina << " " << *obj._sati << ":" << *obj._minute;
		return COUT;
	}
};

class Rezervacija {
	char* _registarskaOznaka;
	TipVozila _tipVozila;
	DatumVrijeme _pocetak;
	int _trajanjeSati;
public:
	Rezervacija(const char* registarskaOznaka, TipVozila tipVozila,
		DatumVrijeme pocetak, int trajanjeSati)
		: _tipVozila(tipVozila), _pocetak(pocetak),
		_trajanjeSati(trajanjeSati) {
		_registarskaOznaka = AlocirajTekst(registarskaOznaka);
	}
	~Rezervacija() { delete[] _registarskaOznaka; _registarskaOznaka = nullptr; }
	const char* GetRegistarskaOznaka() const { return _registarskaOznaka; }
	TipVozila GetTipVozila() const { return _tipVozila; }
	DatumVrijeme& GetPocetak() { return _pocetak; }
	int GetTrajanjeSati() const { return _trajanjeSati; }
	// ToString vraca podatke o rezervaciji u formatu:
	// 10.09.2026 09:00 ABC-123 SUV 5h
	// voditi racuna o prikazu jednocifrenih vrijednosti datuma i vremena (npr. 9 -> 09)
	string ToString() {
		return "";
	}
	// ImaKonfliktSa vraca true ako su rezervacije istog datuma i njihovi vremenski intervali
	// se preklapaju. rezervacija koja pocinje u trenutku kada prethodna zavrsava nije konfliktna.
	bool ImaKonfliktSa(Rezervacija& druga) {
		return false;
	}
};

class Klijent {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	vector<Rezervacija> _rezervacije;
public:
	Klijent(const char* imePrezime = "") {
		_imePrezime = AlocirajTekst(imePrezime);
		_sifra = AlocirajTekst(GenerisiSifru(imePrezime, _id).c_str());
		_id++;
	}
	~Klijent() {
		delete[] _sifra; _sifra = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	const char* GetSifra() const { return _sifra; }
	const char* GetImePrezime() const { return _imePrezime; }
	vector<Rezervacija>& GetRezervacije() { return _rezervacije; }
	// DodajRezervaciju dodaje rezervaciju klijentu ako se ona ne preklapa sa nekom od ranije
	// dodanih rezervacija i ako ukupno trajanje svih rezervacija klijenta u jednom danu ne
	// prelazi 12 sati. metoda vraca true ako je rezervacija dodana, u suprotnom vraca false.
	bool DodajRezervaciju(Rezervacija rezervacija) {
		return false;
	}
	friend ostream& operator<<(ostream& COUT, Klijent& obj) {
		COUT << obj._imePrezime << " [" << obj._sifra << "]" << endl;
		for (auto& rezervacija : obj._rezervacije)
			COUT << " - " << rezervacija.ToString() << endl;
		return COUT;
	}
};
int Klijent::_id = 0;

class RentACarAgencija {
	char* _naziv;
	vector<Klijent> _klijenti;
public:
	RentACarAgencija(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	~RentACarAgencija() { delete[] _naziv; _naziv = nullptr; }
	RentACarAgencija(const RentACarAgencija& obj) {
		_naziv = AlocirajTekst(obj._naziv);
		_klijenti = obj._klijenti;
	}
	const char* GetNaziv() const { return _naziv; }
	vector<Klijent>& GetKlijenti() { return _klijenti; }
	// DodajKlijenta onemogucava dodavanje klijenta sa istom sifrom i baca izuzetak
	void DodajKlijenta(Klijent klijent) {
		_klijenti.push_back(klijent);
	}
	// RegistrujRezervaciju pronalazi klijenta na osnovu sifre i dodaje mu rezervaciju.
	// i dalje vaze pravila definisana u metodi DodajRezervaciju. metoda vraca true ili false.
	bool RegistrujRezervaciju(const char* sifra, Rezervacija rezervacija) {
		return false;
	}
	// AktivniKlijenti vraca pokazivace na klijente koji imaju najmanje onoliko rezervacija
	// koliko je definisano vrijednoscu proslijedjenog parametra.
	vector<Klijent*> AktivniKlijenti(int minBrojRezervacija) {
		vector<Klijent*> rezultat;
		return rezultat;
	}
	// KoristenjePoTipu vraca kolekciju parova (klijent, broj sati) za sve klijente koji
	// imaju najmanje jednu rezervaciju vozila proslijedjenog tipa.
	Kolekcija<Klijent, int, 50> KoristenjePoTipu(TipVozila tip) {
		Kolekcija<Klijent, int, 50> rezultat;
		return rezultat;
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Pojasnite razliku izmedju friend funkcije i clanske funkcije klase ? \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Pojasnite RAII princip i njegovu vezu sa destruktorima ? \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

int main() {
	cout << PORUKA_TELEFON; cin.get();
	cout << PORUKA_ISPIT; cin.get(); system("cls");
	cout << GetOdgovorNaPrvoPitanje() << crt;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << crt;
	cin.get();

	// funkcija generise sifru klijenta na osnovu imena i prezimena, rednog broja i trenutne
	// godine. sifra je u formatu RC-IN-BBB/GGGG, gdje IN predstavlja inicijale, BBB redni broj
	// klijenta popunjen nulama na slobodnim mjestima, a GGGG trenutnu godinu dobijenu iz sistema.
	// funkciju koristiti u konstruktoru klase Klijent za inicijalizaciju atributa _sifra.
	if (GenerisiSifru("Amina Buric", 3) == "RC-AB-003/2026")
		cout << "Sifra OK" << crt;
	if (GenerisiSifru("Amar Macic", 15) == "RC-AM-015/2026")
		cout << "Sifra OK" << crt;
	if (GenerisiSifru("Maid Ramic", 156) == "RC-MR-156/2026")
		cout << "Sifra OK" << crt;

	// koristeci regex, funkcija ValidirajSifru provjerava da li je sifra zapisana u prethodno
	// definisanom formatu. funkcija vraca true ako je sifra validna, u suprotnom vraca false.
	if (ValidirajSifru("RC-AB-003/2026"))
		cout << "SIFRA VALIDNA" << crt;
	if (!ValidirajSifru("RC-Ab-003/2026") && !ValidirajSifru("RC-AB03/2026") &&
		!ValidirajSifru("RC/AB-003-2026"))
		cout << "SIFRA NIJE VALIDNA" << crt;

	Kolekcija<int, string, 20> termini;
	for (int i = 0; i < 8; i++)
		termini.Dodaj(i, "Termin_" + to_string(i));
	cout << termini << crt;

	Kolekcija<int, string, 20> prosireniTermini = termini.DodajNaPoziciju(2, 99, "Poseban termin");
	cout << prosireniTermini << crt;

	Kolekcija<int, string, 20>* uklonjeniTermini = prosireniTermini.UkloniRaspon(3, 2);
	cout << "Uklonjeni elementi:" << crt << *uklonjeniTermini;
	cout << "Preostali elementi:" << crt << prosireniTermini;
	delete uklonjeniTermini;

	try {
		// za neispravan raspon potrebno je baciti izuzetak
		termini.UkloniRaspon(6, 5);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	DatumVrijeme vrijeme1(10, 9, 2026, 9, 0), vrijeme2(10, 9, 2026, 9, 30),
		vrijeme3(10, 9, 2026, 10, 0), vrijeme4(10, 9, 2026, 12, 0);
	Rezervacija auto1("ABC-123", SUV, vrijeme1, 5);
	Rezervacija auto2("XYZ-987", LIMUZINA, vrijeme2, 3);
	Rezervacija auto3("DEF-456", EKONOMSKA, vrijeme3, 4);
	Rezervacija auto4("KOM-789", KOMBI, vrijeme4, 10);

	// ToString metoda vraca podatke o rezervaciji u formatu prikazanom u nastavku.
	// voditi racuna o prikazu jednocifrenih vrijednosti datuma i vremena (npr. 9 -> 09).
	cout << auto1.ToString() << crt;
	// 10.09.2026 09:00 ABC-123 SUV 5h

	if (auto1.ImaKonfliktSa(auto2))
		cout << "Termini se preklapaju" << crt;
	if (!auto1.ImaKonfliktSa(auto3))
		cout << "Termini se ne preklapaju" << crt;

	Klijent amina("Amina Buric"), goran("Goran Skondric"), berun("Berun Agic");

	if (amina.DodajRezervaciju(auto1))
		cout << "Rezervacija dodana" << crt;
	if (!amina.DodajRezervaciju(auto2))
		cout << "Rezervacija nije dodana - preklapanje termina" << crt;
	if (amina.DodajRezervaciju(auto3))
		cout << "Rezervacija dodana" << crt;
	if (!amina.DodajRezervaciju(auto4))
		cout << "Rezervacija nije dodana - prekoracen dnevni limit" << crt;

	RentACarAgencija centarGrad("Centar"), aerodromskaIspostava("Aerodrom");
	centarGrad.DodajKlijenta(amina);
	centarGrad.DodajKlijenta(goran);
	aerodromskaIspostava.DodajKlijenta(berun);

	try {
		// DodajKlijenta onemogucava dodavanje klijenta sa istom sifrom i baca izuzetak
		centarGrad.DodajKlijenta(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	Rezervacija goranovoVozilo("GOR-321", SUV, vrijeme4, 6);
	if (centarGrad.RegistrujRezervaciju(goran.GetSifra(), goranovoVozilo))
		cout << "Rezervacija registrovana" << crt;

	vector<Klijent*> aktivni = centarGrad.AktivniKlijenti(1);
	for (auto klijent : aktivni)
		cout << klijent->GetImePrezime() << " ima " << klijent->GetRezervacije().size() << " rezervacija" << crt;

	Kolekcija<Klijent, int, 50> koristenjeSUV = centarGrad.KoristenjePoTipu(SUV);
	for (int i = 0; i < koristenjeSUV.GetTrenutno(); i++)
		cout << koristenjeSUV.GetPrvi(i).GetImePrezime() << " -> " << koristenjeSUV.GetDrugi(i) << " sati" << crt;

	vector<RentACarAgencija> ispostave;
	ispostave.push_back(centarGrad);
	ispostave.push_back(aerodromskaIspostava);

	/*
	Funkcija UcitajPodatke ucitava podatke o ispostavama i njihovim klijentima iz
	datoteke cije ime se prosljedjuje kao prvi parametar. Svaka linija
	je zapisana u formatu:
	naziv ispostave|ime i prezime klijenta
	Za svaki ispravan red potrebno je:
	- pronaci postojecu ili kreirati novu ispostavu,
	- kreirati i dodati klijenta u odgovarajucu ispostavu,
	- onemoguciti dupliranje ispostava i klijenata unutar iste ispostave.
	Funkcija vraca true ako je ucitan najmanje jedan novi podatak, a false ako datoteka
	ne postoji ili nije ucitan nijedan novi podatak.
	Primjer sadrzaja datoteke:
	Fakultet informacijskih tehnologija
	Pismeni dio ispita iz predmeta Programiranje II
	7
	Centar|Emina Junuz
	Centar|Jasmin Azemovic
	Aerodrom|Zanin Vejzovic
	*/
	if (UcitajPodatke("klijenti.txt", ispostave))
		cout << "Ucitavanje uspjesno" << crt;
	for (auto& ispostava : ispostave)
		cout << ispostava.GetNaziv() << " sa " << ispostava.GetKlijenti().size() << " klijenata" << crt;

	cin.get();
	return 0;
}