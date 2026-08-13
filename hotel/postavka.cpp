#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <regex>
#include <iomanip>
#include <sstream>
#include <fstream>
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
"   OSIM AKO POSTOJI JASNO OPISAN RAZLOG ZA MODIFIKACIJU\n"
"4. IZUZETKE BACAJTE SAMO TAMO GDJE JE IZRICITO NAGLASENO\n"
"5. SVE METODE KOJE SE POZIVAJU U MAIN-U MORAJU POSTOJATI.\n"
"   AKO NEMATE ZELJENU IMPLEMENTACIJU, OSTAVITE PRAZNO TIJELO ILI VRATITE DEFAULT VRIJEDNOST\n"
"6. U MAIN FUNKCIJI MOZETE DODAVATI TESTNE PODATKE I POZIVE PO VLASTITOM IZBORU\n"
"7. TESTIRAJTE PROGRAM U OBA MODA (F5 i Ctrl+F5)" + crt;

char* AlocirajTekst(const char* tekst) {
	if (!tekst) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}

int getTrenutnoGodinuIzSistema() {
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = (timeInfo.tm_year + 1900);
	return godina;
}

enum TipSobe {
	STANDARDNA, APARTMAN, LUKSUZ, STUDIO
};
const char* TipSobeNazivi[] = { "STANDARDNA", "APARTMAN", "LUKSUZ", "STUDIO" };

enum TipGosta {
	REGULARNI, VIP, POSLOVNI, PORODICA
};
const char* TipGostaNazivi[] = { "REGULARNI", "VIP", "POSLOVNI", "PORODICA" };

enum StatusRezervacije {
	KREIRANA, POTVRDJENA, OTPLACENA, ODBIJENA, ZAVRSENA
};
const char* StatusRezervacijeNazivi[] = { "KREIRANA", "POTVRDJENA", "OTPLACENA", "ODBIJENA", "ZAVRSENA" };

// Funkcija generise ID rezervacije u formatu: HRS-IN-BB-0003/2026
// HRS - hotel reservation system
// IN - inicijali (prvo slovo imena i prvo slovo prezimena)
// BB - tip sobe (ST, AP, LK, SD)
// 0003 - redni broj popunjen nulama na 4 mjesta
// 2026 - trenutna godina
// Za neispravne podatke vraca "HRS-XX-XX-0000/0000"
string GenerisiIDRezervacije(const char* imePrezime, int redniBroj, TipSobe tipSobe) {
	string id = "HRS-XX-XX-0000/0000"; // placeholder - implementirati
	return id;
}

// Validira ID rezervacije koristeci regex: HRS-[A-Z]{2}-(ST|AP|LK|SD)-[0-9]{4}/2026
bool ValidirajIDRezervacije(const string& id) {
	return regex_match(id, regex("HRS-[A-Z]{2}-(ST|AP|LK|SD)-[0-9]{4}/2026"));
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _prvi;
	T2* _drugi;
	int _trenutno;
public:
	Kolekcija() : _prvi(nullptr), _drugi(nullptr), _trenutno(0) {}

	Kolekcija(const Kolekcija& obj);

	Kolekcija& operator=(const Kolekcija& obj);

	~Kolekcija();

	int GetTrenutno() const;
	T1& GetPrvi(int indeks);
	T2& GetDrugi(int indeks);
	const T1& GetPrvi(int indeks) const;
	const T2& GetDrugi(int indeks) const;
	T1& operator[](int indeks);

	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj);

	// Dodaje novi par na kraj kolekcije. Ako je kolekcija puna, baca izuzetak.
	void Dodaj(const T1& el1, const T2& el2);

	// Dodaje novi par na poziciju, pomjera postojece udesno i vraca trenutno stanje kolekcije.
	// U slucaju popunjene kolekcije ili neispravne lokacije baca izuzetak.
	Kolekcija<T1, T2, max> DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2);

	// Uklanja raspon elemenata od start do start+brojElemenata-1.
	// Vraca pokazivac na novu kolekciju sa uklonjenim elementima.
	// Za neispravan raspon baca izuzetak.
	Kolekcija<T1, T2, max>* UkloniRaspon(int start, int brojElemenata);
};

class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minute = 0);

	DatumVrijeme(const DatumVrijeme& obj);

	DatumVrijeme& operator=(const DatumVrijeme& obj);

	~DatumVrijeme();

	int getDan() const;
	int getMjesec() const;
	int getGodina() const;
	int getSati() const;
	int getMinute() const;

	// Vraca datum u formatu: DD.MM.GGGG HH:MM (05.10.2026 14:30)
	string ToString() const;

	bool operator>(const DatumVrijeme& obj) const;
	bool operator==(const DatumVrijeme& obj) const;
};

class Osoba {
protected:
	char* _imePrezime;
	char* _email;
	char* _brojTelefona;
public:
	Osoba(const char* imePrezime = "", const char* email = "", const char* brojTelefona = "");

	Osoba(const Osoba& obj);

	Osoba& operator=(const Osoba& obj);

	virtual ~Osoba();

	const char* GetImePrezime() const;
	const char* GetEmail() const;
	const char* GetBrojTelefona() const;

	virtual string ToString() const;

	bool operator==(const Osoba& obj) const;
};

class Gost : public Osoba {
	TipGosta _tipGosta;
	int _brojPosjeta;
	double _ukupnaPotrosnja;
public:
	Gost(const char* imePrezime = "", const char* email = "", const char* brojTelefona = "", 
	     TipGosta tipGosta = REGULARNI, int brojPosjeta = 0, double ukupnaPotrosnja = 0.0);

	Gost(const Gost& obj);

	Gost& operator=(const Gost& obj);

	~Gost();

	TipGosta GetTipGosta() const;
	int GetBrojPosjeta() const;
	double GetUkupnaPotrosnja() const;
	void DodajPosjetu(double iznos);

	// Override ToString - vraca: Ime Prezime | email | TIP | X posjeta | YYYY.ZZ KM
	string ToString() const override;

	// Racuna popust: VIP 20%, POSLOVNI 15%, PORODICA 10%, REGULARNI 0%
	// Dodatni popust na osnovu broja posjeta
	double IzracunajPopust() const;
};

class Rezervacija {
	char* _idRezervacije;
	char* _napomena;
	Gost* _gost;
	TipSobe _tipSobe;
	DatumVrijeme _datumPrijave;
	DatumVrijeme _datumOdjave;
	int _brojGostiju;
	Kolekcija<string, double, 10> _dodatneUsluge;
	StatusRezervacije _status;
public:
	Rezervacija(Gost* gost, TipSobe tipSobe, DatumVrijeme datumPrijave, 
	            DatumVrijeme datumOdjave, int brojGostiju, int redniBroj);

	Rezervacija(const Rezervacija& obj);

	Rezervacija& operator=(const Rezervacija& obj);

	~Rezervacija();

	const char* GetIdRezervacije() const;
	const char* GetNapomena() const;
	void SetNapomena(const char* napomena);
	Gost* GetGost() const;
	TipSobe GetTipSobe() const;
	DatumVrijeme& GetDatumPrijave();
	const DatumVrijeme& GetDatumPrijave() const;
	DatumVrijeme& GetDatumOdjave();
	const DatumVrijeme& GetDatumOdjave() const;
	int GetBrojGostiju() const;
	Kolekcija<string, double, 10>& GetDodatneUsluge();
	StatusRezervacije GetStatus() const;

	friend ostream& operator<<(ostream& COUT, const Rezervacija& obj);

	// Dodaje dodatnu uslugu. Ako vec postoji, vraca false.
	bool DodajDodatnuUslugu(const string& naziv, double cijena);

	// Racuna ukupnu cijenu: cijena sobe po noci * broj nocenja + dodatne usluge - popust
	double IzracunajUkupnuCijenu() const;

	// Mijenja status: KREIRANA -> POTVRDJENA -> OTPLACENA -> ZAVRSENA
	// ODBIJENA iz KREIRANA ili POTVRDJENA
	bool PromijeniStatus(StatusRezervacije noviStatus);

	// Vraca: ID | Ime | TIP | datum prijave - datum odjave | X gostiju | STATUS
	string ToString() const;
};

class Hotel {
	char* _naziv;
	char* _adresa;
	vector<Gost> _gosti;
	vector<Rezervacija> _rezervacije;
public:
	Hotel(const char* naziv = "", const char* adresa = "");

	Hotel(const Hotel& obj);

	Hotel& operator=(const Hotel& obj);

	~Hotel();

	const char* GetNaziv() const;
	const char* GetAdresa() const;
	vector<Gost>& GetGosti();
	vector<Rezervacija>& GetRezervacije();

	// Dodaje gosta. Ako gost sa istim email-om vec postoji, baca izuzetak.
	void DodajGosta(const Gost& gost);

	// Kreira rezervaciju za gosta pronadjenog po email-u. Vraca true ako je uspjesno.
	bool KreirajRezervaciju(const char* emailGosta, TipSobe tipSobe, 
	                       DatumVrijeme datumPrijave, DatumVrijeme datumOdjave, 
	                       int brojGostiju);

	// Vraca pokazivace na goste koji imaju barem minRezervacija rezervacija.
	vector<Gost*> AktivniGosti(int minRezervacija);

	// Vraca kolekciju parova (gost, ukupna potrosnja) za goste sa rezervacijama odredjenog tipa sobe.
	Kolekcija<Gost, double, 50> GostiPoTipuSobe(TipSobe tipSobe);
};

bool UcitajPodatke(const string& putanja, vector<Hotel>& hoteli) {
	// Ucitava podatke iz datoteke u formatu:
	// naziv hotela|adresa|ime i prezime gosta|email|broj telefona|tip gosta|broj posjeta
	// Vraca true ako je ucitan barem jedan novi podatak, false ako datoteka ne postoji ili nema novih podataka
	return false;
}

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Objasnite koncept virtuelnih funkcija i polimorfizma u C++-u. Koja je uloga virtualnog destruktora? Navedite primjer.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Sta su smart pointeri u C++-u i koje vrste postoje? Objasnite prednosti koristenja smart pointera u odnosu na obicne pokazivace.\n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

int main() {
	cout << PORUKA_TELEFON; cin.get();
	cout << PORUKA_ISPIT; cin.get(); system("cls");
	cout << GetOdgovorNaPrvoPitanje() << crt;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << crt;
	cin.get();

	//funkcija generise ID rezervacije na osnovu imena i prezimena, rednog broja, tipa sobe i trenutne godine.
	//ID je u formatu HRS-IN-BB-0003/2026, gdje IN predstavlja inicijale, BB tip sobe (ST, AP, LK, SD),
	//0003 redni broj popunjen nulama na cetiri mjesta, a 2026 trenutnu godinu.
	//funkciju koristiti u konstruktoru klase Rezervacija za inicijalizaciju atributa _idRezervacije.
	if (GenerisiIDRezervacije("Amina Buric", 3, APARTMAN) == "HRS-AB-AP-0003/2026")
		cout << "ID rezervacije OK" << crt;
	if (GenerisiIDRezervacije("Amar Macic", 15, STANDARDNA) == "HRS-AM-ST-0015/2026")
		cout << "ID rezervacije OK" << crt;
	if (GenerisiIDRezervacije("Maid Ramic", 156, LUKSUZ) == "HRS-MR-LK-0156/2026")
		cout << "ID rezervacije OK" << crt;
	if (GenerisiIDRezervacije("Amina", 42, APARTMAN) == "HRS-XX-XX-0000/0000" &&
		GenerisiIDRezervacije("Amina Buric", 0, APARTMAN) == "HRS-XX-XX-0000/0000" &&
		GenerisiIDRezervacije("Amina Buric", 10000, APARTMAN) == "HRS-XX-XX-0000/0000" &&
		GenerisiIDRezervacije("Amina Buric", 42, (TipSobe)99) == "HRS-XX-XX-0000/0000")
		cout << "Neispravni podaci za ID OK" << crt;

	//ValidirajIDRezervacije koristeci regex provjerava format definisan u prethodnom dijelu zadatka.
	if (ValidirajIDRezervacije("HRS-AB-AP-0003/2026"))
		cout << "ID REZERVACIJE VALIDAN" << crt;
	if (!ValidirajIDRezervacije("HRS-Ab-AP-0003/2026") &&
		!ValidirajIDRezervacije("HRS-AB-XX-0003/2026") &&
		!ValidirajIDRezervacije("HRS-AB-AP-003/2026") &&
		!ValidirajIDRezervacije("HRS-AB-AP-0003-2026") &&
		!ValidirajIDRezervacije("HRS-AB-AP-0000/2026"))
		cout << "ID REZERVACIJE NIJE VALIDAN" << crt;

	Kolekcija<int, string, 20> usluge;
	for (int i = 0; i < 8; i++)
		usluge.Dodaj(i, "Usluga_" + to_string(i));
	cout << usluge << crt;

	//DodajNaPoziciju dodaje novi par na lokaciju definisanu prvim parametrom.
	//Metoda vraca trenutno stanje kolekcije.
	//U slucaju popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak.
	Kolekcija<int, string, 20> prosireneUsluge = usluge.DodajNaPoziciju(2, 99, "SPA tretman");
	cout << prosireneUsluge << crt;

	//UkloniRaspon uklanja broj elemenata definisan drugim parametrom, pocevsi od lokacije
	//definisane prvim parametrom (ukljucujuci tu lokaciju). Metoda vraca pokazivac na novu kolekciju
	//sa uklonjenim elementima, a pozivalac je odgovoran za njenu dealokaciju.
	Kolekcija<int, string, 20>* uklonjene = prosireneUsluge.UkloniRaspon(3, 2);
	cout << "Uklonjene usluge:" << crt << *uklonjene;
	cout << "Preostale usluge:" << crt << prosireneUsluge;
	delete uklonjene;

	try {
		//za neispravan raspon potrebno je baciti izuzetak
		usluge.UkloniRaspon(6, 5);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	DatumVrijeme prijava(10, 9, 2026, 14, 0);
	DatumVrijeme odjava(15, 9, 2026, 10, 0);

	//ToString metoda vraca datum i vrijeme u formatu DD.MM.GGGG HH:MM.
	//Voditi racuna o prikazu jednocifrenih vrijednosti (npr. 5 -> 05).
	cout << prijava.ToString() << crt; // 10.09.2026 14:00

	if (odjava > prijava)
		cout << "Odjava je poslije prijave" << crt;

	Gost amina("Amina Buric", "amina@fit.ba", "061123456", VIP, 5, 1500.0);
	Gost goran("Goran Skondric", "goran@fit.ba", "061654321", POSLOVNI, 2, 300.0);

	//ToString metoda klase Gost vraca podatke o gostu u formatu:
	//Ime Prezime | email | TIP_GOSTA | X posjeta | YYYY.ZZ KM
	cout << amina.ToString() << crt;

	//IzracunajPopust racuna popust na osnovu tipa gosta i broja posjeta.
	cout << "Popust za Aminu: " << amina.IzracunajPopust() * 100 << "%" << crt;

	Rezervacija rez1(&amina, APARTMAN, prijava, odjava, 2, 1);
	rez1.DodajDodatnuUslugu("Dorucak", 25.0);
	rez1.DodajDodatnuUslugu("Parking", 10.0);

	//ToString metoda klase Rezervacija vraca podatke u formatu:
	//HRS-AB-AP-0001/2026 | Amina Buric | APARTMAN | 10.09.2026 - 15.09.2026 | 2 gosta | KREIRANA
	cout << rez1.ToString() << crt;

	//IzracunajUkupnuCijenu racuna ukupnu cijenu rezervacije ukljucujuci cijenu sobe,
	//dodatne usluge i popust gosta.
	cout << "Ukupna cijena: " << rez1.IzracunajUkupnuCijenu() << "KM" << crt;

	//PromijeniStatus mijenja status rezervacije prema dozvoljenom slijedu:
	//KREIRANA -> POTVRDJENA -> OTPLACENA -> ZAVRSENA
	//ODBIJENA se moze postaviti iz KREIRANA ili POTVRDJENA
	rez1.PromijeniStatus(POTVRDJENA);
	rez1.PromijeniStatus(OTPLACENA);
	if (!rez1.PromijeniStatus(ODBIJENA))
		cout << "Status nije promijenjen - neispravan slijed" << crt;
	rez1.PromijeniStatus(ZAVRSENA);

	Hotel hotel("Holiday Inn", "Zmaja od Bosne 123");

	//DodajGosta dodaje gosta u hotel. Onemogucava dodavanje gosta sa istim email-om i baca izuzetak.
	hotel.DodajGosta(amina);
	hotel.DodajGosta(goran);

	try {
		hotel.DodajGosta(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	DatumVrijeme prijava2(15, 9, 2026, 14, 0);
	DatumVrijeme odjava2(18, 9, 2026, 10, 0);

	//KreirajRezervaciju pronalazi gosta na osnovu email-a i kreira rezervaciju.
	//Vraca true ako je rezervacija kreirana, inace false.
	if (hotel.KreirajRezervaciju("goran@fit.ba", STANDARDNA, prijava2, odjava2, 1))
		cout << "Rezervacija kreirana" << crt;

	//AktivniGosti vraca pokazivace na goste koji imaju najmanje onoliko rezervacija
	//koliko je definisano vrijednoscu proslijedjenog parametra.
	vector<Gost*> aktivni = hotel.AktivniGosti(1);
	for (auto gost : aktivni)
		cout << gost->GetImePrezime() << " ima aktivnih rezervacija" << crt;

	//GostiPoTipuSobe vraca kolekciju parova (gost, ukupna potrosnja) za sve goste
	//koji imaju barem jednu rezervaciju sobe proslijedjenog tipa.
	Kolekcija<Gost, double, 50> gostiApartmana = hotel.GostiPoTipuSobe(APARTMAN);
	for (int i = 0; i < gostiApartmana.GetTrenutno(); i++)
		cout << gostiApartmana.GetPrvi(i).GetImePrezime() << " -> " << gostiApartmana.GetDrugi(i) << " KM" << crt;

	vector<Hotel> hoteli;
	hoteli.push_back(hotel);

	/*
	Funkcija UcitajPodatke ucitava podatke o hotelima i njihovim gostima iz datoteke
	cije ime se prosljedjuje kao prvi parametar. Svaka linija je zapisana u formatu:
	naziv hotela|adresa|ime i prezime gosta|email|broj telefona|tip gosta|broj posjeta
	Za svaki ispravan red potrebno je:
	- pronaci postojeci ili kreirati novi hotel,
	- kreirati i dodati gosta u odgovarajuci hotel,
	- onemoguciti dupliranje hotela i gostiju unutar istog hotela.
	Funkcija vraca true ako je ucitan najmanje jedan novi podatak, a false ako datoteka ne
	postoji ili nije ucitan nijedan novi podatak.
	*/
	if (UcitajPodatke("gosti.txt", hoteli))
		cout << "Ucitavanje uspjesno" << crt;
	for (auto& h : hoteli)
		cout << h.GetNaziv() << " sa " << h.GetGosti().size() << " gostiju" << crt;

	cin.get();
	return 0;
}