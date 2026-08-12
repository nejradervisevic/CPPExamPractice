#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <string>
#include <regex>
#include <ctime>
using namespace std;
string crt = "\n---------------------------------------------------------------------------------\n";

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
string GenerisiOznaku(const char* vrsta, int redniBroj, int godina) {
	if (vrsta == nullptr || strlen(vrsta) < 1) return "Z-000/XX-0000";
	if (redniBroj < 1 || redniBroj > 999) return "Z-000/XX-0000";
	if (godina < 2000 || godina > 2099) return "Z-000/XX-0000";

	string oznaka;
	oznaka += "Z-";

	if (redniBroj < 10) oznaka += "00";
	else if (redniBroj < 100) oznaka += "0";
	oznaka += to_string(redniBroj);
	oznaka += "/";

	oznaka += toupper(vrsta[0]);
	const char* razmak = strchr(vrsta, ' ');
	if (razmak != nullptr && strlen(razmak) > 1) oznaka += toupper(razmak[1]);
	if (razmak == nullptr) return "Z-000/XX-0000";
	oznaka += "-";

	oznaka += to_string(godina);

	return oznaka;
}
bool ValidirajOznaku(const string& oznaka) {
	return regex_match(oznaka, regex("Z-(?!000)\\d{3}/[A-Z]{2}-20[0-9]{2}"));
}

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
	Kolekcija(const Kolekcija& obj) {
		_trenutno = obj.GetTrenutno();
		_elementi1 = new T1[max];
		_elementi2 = new T2[max];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.GetElement1(i);
			_elementi2[i] = obj.GetElement2(i);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete[] _elementi1;
			delete[] _elementi2;
			_elementi1 = nullptr;
			_elementi2 = nullptr;
			_trenutno = 0;

			_trenutno = obj.GetTrenutno();
			_elementi1 = new T1[max];
			_elementi2 = new T2[max];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj.GetElement1(i);
				_elementi2[i] = obj.GetElement2(i);
			}
		}
		return *this;
	}
	int GetTrenutno() const { return _trenutno; }
	const T1& GetElement1(int indeks) const { return _elementi1[indeks]; }
	const T2& GetElement2(int indeks) const { return _elementi2[indeks]; }
	T1& GetElement1(int indeks) { return _elementi1[indeks]; }
	T2& GetElement2(int indeks) { return _elementi2[indeks]; }
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
	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max) throw exception("Kolekcija je puna");

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;

		_trenutno++;
	}
	Kolekcija UkloniDuplikate() {
		Kolekcija temp;
		for (size_t i = 0; i < _trenutno; i++)
		{
			bool duplikat = false;
			for (size_t j = 0; j < temp.GetTrenutno(); j++)
			{
				if (_elementi1[i] == temp.GetElement1(j) && _elementi2[i] == temp.GetElement2(j)) duplikat = true;
			}
			if (!duplikat) temp.Dodaj(_elementi1[i], _elementi2[i]);
		}
		return temp;
	}
	T1& operator[](int indeks) const { return _elementi1[indeks]; };
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
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _dan;
			delete _mjesec;
			delete _godina;
			delete _sati;
			delete _minute;
			_dan = _mjesec = _godina = _sati = _minute = nullptr;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
			_sati = new int(*obj._sati);
			_minute = new int(*obj._minute);
		}
		return *this;
	}
	int GetGodina() const { return *_godina; }
	int GetMjesec() const { return *_mjesec; }
	int GetDan() const { return *_dan; }
	int GetSati() const { return *_sati; }
	int GetMinute() const { return *_minute; }
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
	string ToString() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << *_dan << "."
			<< setw(2) << setfill('0') << *_mjesec << "."
			<< setw(2) << setfill('0') << *_godina << " "
			<< setw(2) << setfill('0') << *_sati << ":"
			<< setw(2) << setfill('0') << *_minute;

		return ss.str();
	}
	bool operator>(const DatumVrijeme obj) const {
		if (*_godina > obj.GetGodina()) return true;
		if (*_godina < obj.GetGodina()) return false;

		if (*_mjesec > obj.GetMjesec()) return true;
		if (*_mjesec < obj.GetMjesec()) return false;

		if (*_dan > obj.GetDan()) return true;
		if (*_dan < obj.GetDan()) return false;

		if (*_sati > obj.GetSati()) return true;
		if (*_sati < obj.GetSati()) return false;

		if (*_minute > obj.GetMinute()) return true;
		if (*_minute < obj.GetMinute()) return false;

		return false;
	}
	bool operator==(const DatumVrijeme& obj) const {
		return *_godina == obj.GetGodina() && *_mjesec == obj.GetMjesec() && *_dan == obj.GetDan() && *_sati == obj.GetSati() && *_minute == obj.GetMinute();
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
	string ToString() const {
		stringstream ss;

		ss << _opis << " | " << _veterinar << " | " << _cijena << " KM | " << _trajanjeMinuta << " min";

		return ss.str();
	}
	Pregled(string opis = "", string veterinar = "", double cijena = 0.00, int trajanjeMinuta = 0) {
		_opis = opis;
		_veterinar = veterinar;
		_cijena = cijena;
		_trajanjeMinuta = trajanjeMinuta;
	}
	Pregled(const Pregled& obj) {
		_opis = obj.GetOpis();
		_veterinar = obj.GetVeterinar();
		_cijena = obj.GetCijena();
		_trajanjeMinuta = obj.GetTrajanjeMinuta();
	}
	Pregled& operator=(const Pregled& obj) {
		if (this != &obj)
		{
			_opis = obj.GetOpis();
			_veterinar = obj.GetVeterinar();
			_cijena = obj.GetCijena();
			_trajanjeMinuta = obj.GetTrajanjeMinuta();
		}
		return *this;
	}
	~Pregled() {};
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
	Cuvar(string imePrezime = "", string email = "", string telefon = "") {
		_imePrezime = imePrezime;
		_email = email;
		_telefon = telefon;
	}
	Cuvar(const Cuvar& obj) {
		_imePrezime = obj.GetImePrezime();
		_email = obj.GetEmail();
		_telefon = obj.GetTelefon();
	}
	Cuvar& operator=(const Cuvar& obj) {
		if (this != &obj)
		{
			_imePrezime = obj.GetImePrezime();
			_email = obj.GetEmail();
			_telefon = obj.GetTelefon();
		}
		return *this;
	}
	~Cuvar() {};
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
	string ToString() const {
		stringstream ss;

		ss << _oznaka << " | " << _cuvar.GetImePrezime() << " | " << _vrsta << " | " << StatusNastanjivanjaNazivi[_statusi.GetTrenutno() - 1];

		return ss.str();
	}
	Zivotinja(const char* vrsta, const char* opisStanja, const Cuvar& cuvar, int redniBroj, const DatumVrijeme& vrijeme) {
		_oznaka = AlocirajTekst(GenerisiOznaku(vrsta, redniBroj, vrijeme.GetGodina()).c_str());

		_vrsta = AlocirajTekst(vrsta);
		_opisStanja = AlocirajTekst(opisStanja);
		_redniBroj = redniBroj;
		_cuvar = cuvar;

		_statusi.Dodaj(NOVOPRIDOSLA, vrijeme);
	}
	Zivotinja(const Zivotinja& obj) {
		_oznaka = AlocirajTekst(obj.GetOznaka());
		_vrsta = AlocirajTekst(obj.GetVrsta());
		_opisStanja = AlocirajTekst(obj.GetOpisStanja());
		_redniBroj = obj.GetRedniBroj();
		_cuvar = obj.GetCuvar();
		_statusi = obj.GetStatusi();
		_pregledi = obj.GetPregledi();
	}
	Zivotinja& operator=(const Zivotinja& obj) {
		if (this != &obj)
		{
			delete[] _oznaka;
			delete[] _vrsta;
			delete[] _opisStanja;
			_oznaka = _vrsta = _opisStanja = nullptr;

			_oznaka = AlocirajTekst(obj.GetOznaka());
			_vrsta = AlocirajTekst(obj.GetVrsta());
			_opisStanja = AlocirajTekst(obj.GetOpisStanja());
			_redniBroj = obj.GetRedniBroj();
			_cuvar = obj.GetCuvar();
			_statusi = obj.GetStatusi();
			_pregledi = obj.GetPregledi();
		}
		return *this;
	}
	~Zivotinja() {
		delete[] _oznaka;
		delete[] _vrsta;
		delete[] _opisStanja;
		_oznaka = _vrsta = _opisStanja = nullptr;
	}
	bool DodajStatus(StatusNastanjivanja noviStatus, const DatumVrijeme& vrijeme) {
		DatumVrijeme& posljednjeVrijeme = _statusi.GetElement2(_statusi.GetTrenutno() - 1);
		if (posljednjeVrijeme > vrijeme) return false;
		if (posljednjeVrijeme == vrijeme) return false;

		StatusNastanjivanja& trenutniStatus = _statusi.GetElement1(_statusi.GetTrenutno() - 1);

		if (trenutniStatus == noviStatus) return false;

		if (trenutniStatus == NOVOPRIDOSLA) {
			if (noviStatus != KARANTIN) return false;
		}
		if (trenutniStatus == KARANTIN) {
			if (noviStatus != PRILAGODJAVANJE) return false;
		}
		if (trenutniStatus == PRILAGODJAVANJE) {
			if (noviStatus != NASTANJENA) return false;
		}
		if (trenutniStatus == NASTANJENA) return false;

		_statusi.Dodaj(noviStatus, vrijeme);
		return true;
	}
	bool DodajPregled(const Pregled& pregled) {
		StatusNastanjivanja& trenutniStatus = _statusi.GetElement1(_statusi.GetTrenutno() - 1);   

		if (trenutniStatus != KARANTIN && trenutniStatus != PRILAGODJAVANJE) return false;
		if (pregled.GetCijena() <= 0 || pregled.GetTrajanjeMinuta() <= 0) return false;

		_pregledi.push_back(pregled);
		return true;
	}		
	double UkupniTroskovi() {
		double ukupniTroskovi = 0.00;
		for (size_t i = 0; i < _pregledi.size(); i++)
		{
			ukupniTroskovi += _pregledi[i].GetCijena();
		}
		return ukupniTroskovi;
	}
	int UkupnoTrajanje() {
		int ukupnoTrajanje = 0;
		for (size_t i = 0; i < _pregledi.size(); i++)
		{
			ukupnoTrajanje += _pregledi[i].GetTrajanjeMinuta();
		}
		return ukupnoTrajanje;
	}
	bool operator==(const Zivotinja& obj) const {
		return strcmp(_oznaka, obj.GetOznaka()) == 0 && _redniBroj == obj.GetRedniBroj();
 	}
};
class Zoo {
	char* _naziv;
	vector<Zivotinja> _zivotinje;
public:
	Zoo(const char* naziv = "") {
		_naziv = AlocirajTekst(naziv);
	}
	Zoo(const Zoo& obj) {
		_naziv = AlocirajTekst(obj.GetNaziv());
		_zivotinje = obj.GetZivotinje();
	}
	Zoo& operator=(const Zoo& obj) {
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj.GetNaziv());
			_zivotinje = obj.GetZivotinje();
		}
		return *this;
	}
	const char* GetNaziv() const { return _naziv; }
	vector<Zivotinja>& GetZivotinje() { return _zivotinje; }
	const vector<Zivotinja>& GetZivotinje() const { return _zivotinje; }
	~Zoo() {
		delete[] _naziv; _naziv = nullptr;
	}
	void DodajZivotinju(const Zivotinja& zivotinja) {
		for (auto& z : _zivotinje) {
			if (z == zivotinja) throw exception("Zivotinja je vec primljena u ZOO!");
		}
		_zivotinje.push_back(zivotinja);
	}
	Zivotinja* PronadjiZivotinju(const string& oznaka) {
		for (auto& z : _zivotinje) {
			if (z.GetOznaka() == oznaka) return &z;
		}
		return nullptr;
	}
	bool EvidentirajStatus(const string& oznaka, StatusNastanjivanja noviStatus, const DatumVrijeme& vrijeme) {
		Zivotinja* zivotinja = PronadjiZivotinju(oznaka);

		if (zivotinja == nullptr) return false;

		if (!zivotinja->DodajStatus(noviStatus, vrijeme)) return false;

		if (noviStatus == NASTANJENA) {
			thread t([&] {
				cout << crt;
				cout << "To: " << zivotinja->GetCuvar() << endl;
				cout << "From: uprava@zoo.ba" << endl;
				cout << "Subject: Zivotinja nastanjena - izvjestaj" << endl;
				cout << "Postovani/a Amina Buric, " << endl;
				cout << "Zivotinja " << zivotinja->GetOznaka() << " " << "(" << zivotinja->GetVrsta() << ")" << " je uspjesno nastanjena" << endl;
				cout << "Ukupni troskovi pregleda: " << zivotinja->UkupniTroskovi() << " KM" << endl;
				cout << "Hvala na brizi." << endl;
				cout << crt;
			});

			t.join();
		}
		return true;
	}
	vector<Zivotinja*> IzdvojiNenastanjene() {
	/*IzdvojiNenastanjene vraca vector pokazivaca na sve zivotinje ciji
	trenutni status nije NASTANJENA.*/
		vector<Zivotinja*> temp;
		for (size_t i = 0; i < _zivotinje.size(); i++)
		{
			if (_zivotinje[i].GetTrenutniStatus() != NASTANJENA) temp.push_back(&_zivotinje[i]);
		}
		return temp;
	}
	double IzracunajTroskove() {
	/*IzracunajTroskove vraca zbir cijena svih pregleda evidentiranih na
	zivotinjama koje imaju status NASTANJENA. Pregledi na jos nenastanjenim
	zivotinjama ne ulaze u zbir.*/
		double troskovi = 0.00;
		for (size_t i = 0; i < _zivotinje.size(); i++)
		{
			if (_zivotinje[i].GetTrenutniStatus() == NASTANJENA) troskovi += _zivotinje[i].UkupniTroskovi();
		}
		return troskovi;
	}
};

int main() {
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

	/*Koristeci regex, funkcija ValidirajOznaku provjerava prethodno definisani format.
	Prefiks mora biti Z, redni broj mora imati tri cifre i ne moze biti 000,
	inicijali moraju biti velika slova, a godina je 2000-2099.
	Potpis funkcije treba biti:
	bool ValidirajOznaku(const string& oznaka)*/

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
	cout << dosla.ToString() << crt; 

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
	
	/*ToString vraca podatke u formatu:
	oznaka | ime i prezime cuvara | vrsta | trenutni status
	Z-015/PT-2026 | Amina Buric | Panthera tigris | NOVOPRIDOSLA*/
	cout << tigar.ToString() << crt;
	if (tigar.ToString() == "Z-015/PT-2026 | Amina Buric | Panthera tigris | NOVOPRIDOSLA")
		cout << "Zivotinja ToString OK" << crt;

	/*DodajStatus dodaje status samo ako je vrijeme vece od vremena posljednjeg
	statusa i ako status predstavlja neposredno sljedecu fazu.
	Dozvoljen slijed je: NOVOPRIDOSLA -> KARANTIN -> PRILAGODJAVANJE -> NASTANJENA
	Preskakanje ili ponavljanje statusa nije dozvoljeno. Nakon statusa
	NASTANJENA nisu dozvoljene nove promjene. Neuspjesan pokusaj vraca false
	bez izmjene kolekcije statusa.*/
	if (!tigar.DodajStatus(PRILAGODJAVANJE, karantin))
		cout << "Preskakanje statusa nije dozvoljeno" << crt;
	if (tigar.DodajStatus(KARANTIN, karantin))
		cout << "Status KARANTIN dodan" << crt;
	if (!tigar.DodajStatus(KARANTIN, prilagodjavanje))
		cout << "Ponavljanje statusa nije dozvoljeno" << crt;

	/*DodajPregled dodaje pregled samo dok je zivotinja u statusu
	KARANTIN ili PRILAGODJAVANJE, uz cijenu i trajanje vece od nule.
	Metoda vraca true ako je pregled dodan, u suprotnom false.
	UkupniTroskovi vraca zbir cijena svih pregleda, a UkupnoTrajanje
	zbir njihovih trajanja u minutama.*/
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
	
	/*DodajZivotinju dodaje zivotinju u zoo. Nije dozvoljeno dodati dvije zivotinje
	sa istim rednim brojem ili istom oznakom. U slucaju duplikata metoda baca izuzetak.*/
	sarajevskiZoo.DodajZivotinju(tigar);
	sarajevskiZoo.DodajZivotinju(orao);
	try {
		sarajevskiZoo.DodajZivotinju(orao);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	/*PronadjiZivotinju vraca pokazivac na zivotinju sa proslijedjenom oznakom.
	Ako zivotinja nije pronadjena, metoda vraca nullptr.*/
	string oznakaTigra = tigar.GetOznaka();
	Zivotinja* pronadjena = sarajevskiZoo.PronadjiZivotinju(oznakaTigra);
	if (pronadjena != nullptr)
		cout << "Pronadjena zivotinja: " << pronadjena->GetOznaka() << crt;
	if (sarajevskiZoo.PronadjiZivotinju("Z-999/XX-2026") == nullptr)
		cout << "Zivotinja nije pronadjena" << crt;

	/*EvidentirajStatus pronalazi zivotinju i pokusava dodati status
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
	---------------------------------------------------------------------------------*/
	if (sarajevskiZoo.EvidentirajStatus(oznakaTigra, NASTANJENA, nastanjena))
		cout << "Zivotinja nastanjena i izvjestaj poslan" << crt;
	if (!sarajevskiZoo.EvidentirajStatus(oznakaTigra, NASTANJENA, DatumVrijeme(21, 5, 2026, 9, 0)))
		cout << "Ponavljanje zavrsnog statusa nije dozvoljeno" << crt;
	if (!sarajevskiZoo.EvidentirajStatus("NEPOSTOJECA", KARANTIN, karantin))
		cout << "Status nije evidentiran za nepostojecu zivotinju" << crt;

	/*IzdvojiNenastanjene vraca vector pokazivaca na sve zivotinje ciji
	trenutni status nije NASTANJENA.*/
	vector<Zivotinja*> nenastanjene = sarajevskiZoo.IzdvojiNenastanjene();
	for (auto zivotinja : nenastanjene)
		cout << zivotinja->GetOznaka() << " -> " << StatusNastanjivanjaNazivi[(int)zivotinja->GetTrenutniStatus()] << crt;

	/*IzracunajTroskove vraca zbir cijena svih pregleda evidentiranih na
	zivotinjama koje imaju status NASTANJENA. Pregledi na jos nenastanjenim
	zivotinjama ne ulaze u zbir.*/
	cout << "Ukupni troskovi nastanjivanja: " << sarajevskiZoo.IzracunajTroskove() << " KM" << crt;
	Zoo kopijaZoo = sarajevskiZoo;
	cout << kopijaZoo.GetNaziv() << " ima " << kopijaZoo.GetZivotinje().size() << " zivotinja" << crt;

	cin.get();
	return 0;
}