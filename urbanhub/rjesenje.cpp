#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <regex>
#include <thread>
using namespace std;
const char* crt = "\n---------------------------------------------------------------------------------\n";

char* AlocirajTekst(const char* tekst) {
	if (tekst == nullptr) return nullptr;
	size_t velicina = strlen(tekst) + 1;
	char* noviTekst = new char[velicina];
	strcpy_s(noviTekst, velicina, tekst);
	return noviTekst;
}
enum TipProstorije { OTVORENI_PROSTOR, SALA_ZA_SASTANKE, PRIVATNA_KANCELARIJA, TELEFONSKA_KABINA };
const char* TipProstorijeNazivi[] = {
"OTVORENI PROSTOR", "SALA ZA SASTANKE", "PRIVATNA KANCELARIJA", "TELEFONSKA KABINA" };
string GenerisiOznakuProstorije(const char* naziv, int kapacitet, int sprat) {
	if (naziv == nullptr || strlen(naziv) < 1) return "CW-000/XX-X";
	if (kapacitet < 1 || kapacitet > 500) return "CW-000/XX-X";
	if (sprat < 0 || sprat > 9) return "CW-000/XX-X";

	string oznaka;
	oznaka += "CW-";

	if (kapacitet < 10) oznaka += "00";
	else if (kapacitet < 100) oznaka += "0";
	oznaka += to_string(kapacitet);
	oznaka += "/";

	oznaka += toupper(naziv[0]);
	const char* razmak = strchr(naziv, ' ');
	if (razmak != nullptr && strlen(razmak) > 1) oznaka += toupper(razmak[1]);
	if (razmak == nullptr) return "CW-000/XX-X";
	oznaka += "-";

	oznaka += to_string(sprat);

	return oznaka;
}
bool ValidirajOznakuProstorije(const string& oznaka) {
	return regex_match(oznaka, regex("CW-(?!000)\\d{3}/[A-Z]{2}-\\d"));
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

		_elementi1 = new T1[_trenutno];
		_elementi2 = new T2[_trenutno];

		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = obj.GetElement1(i);
			_elementi2[i] = obj.GetElement2(i);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete[] _elementi1; delete[] _elementi2;
			_elementi1 = nullptr; _elementi2 = nullptr;

			_trenutno = obj.GetTrenutno();

			_elementi1 = new T1[_trenutno];
			_elementi2 = new T2[_trenutno];

			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = obj.GetElement1(i);
				_elementi2[i] = obj.GetElement2(i);
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[] _elementi1; delete[] _elementi2;
		_elementi1 = nullptr; _elementi2 = nullptr;
	}
	int GetTrenutno() const { return _trenutno; }
	T1& GetElement1(int i) { return _elementi1[i]; }
	T2& GetElement2(int i) { return _elementi2[i]; }
	const T1& GetElement1(int i) const { return _elementi1[i]; }
	const T2& GetElement2(int i) const { return _elementi2[i]; }
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetElement1(i) << " " << fixed << setprecision(2)
			<< obj.GetElement2(i) << endl;
		return COUT;
	}
	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max) throw exception("Kolekcija je puna");

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;

		_trenutno++;
	}
	Kolekcija IzdvojiIznadVrijednosti(const T2& granica) {
		Kolekcija temp;
		for (int i = 0; i < _trenutno; i++)
		{
			if (_elementi2[i] >= granica) temp.Dodaj(_elementi1[i], _elementi2[i]);
		}
		return temp;
	}
};
class DatumVrijeme {
	int* _dan, * _mjesec, * _godina, * _sati, * _minute;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minute = 0) {
		_dan = new int(dan); _mjesec = new int(mjesec); _godina = new int(godina);
		_sati = new int(sati); _minute = new int(minute);
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan); _mjesec = new int(*obj._mjesec); _godina = new int(*obj._godina);
		_sati = new int(*obj._sati); _minute = new int(*obj._minute);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
			_dan = _mjesec = _godina = _sati = _minute = nullptr;

			_dan = new int(*obj._dan); _mjesec = new int(*obj._mjesec); _godina = new int(*obj._godina);
			_sati = new int(*obj._sati); _minute = new int(*obj._minute);
		}
		return *this;
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
	string ToString() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << *_dan << "."
			<< setw(2) << setfill('0') << *_mjesec << "."
			<< setw(2) << setfill('0') << *_godina << " "
			<< setw(2) << setfill('0') << *_sati << ":"
			<< setw(2) << setfill('0') << *_minute;

		return ss.str();
	}
	friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj) {
		COUT << obj.ToString() << endl;
		return COUT;
	}
	bool DaLiJeDatumIsti(const DatumVrijeme& noviDatum) const {
		return *_dan == *noviDatum._dan &&
			*_mjesec == *noviDatum._mjesec &&
			*_godina == *noviDatum._godina;
	}
};
class Rezervacija {
protected:
	DatumVrijeme _pocetak;
	int _trajanjeMinuta;
public:
	Rezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta)
		: _pocetak(pocetak), _trajanjeMinuta(trajanjeMinuta) {
	}
	Rezervacija(const Rezervacija& obj) {
		_pocetak = obj.GetPocetak();
		_trajanjeMinuta = obj.GetTrajanjeMinuta();
	}
	Rezervacija& operator=(const Rezervacija& obj) {
		if (this != &obj)
		{
			_pocetak = obj.GetPocetak();
			_trajanjeMinuta = obj.GetTrajanjeMinuta();
		}
		return *this;
	}
	virtual ~Rezervacija() {}
	virtual string Info() const = 0;
	virtual Rezervacija* GetClone() const = 0;
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeMinuta() const { return _trajanjeMinuta; }
	bool DaLiImaKonflikt(const Rezervacija& rezervacija) {
		if (!_pocetak.DaLiJeDatumIsti(rezervacija.GetPocetak())) return false;

		int pocetakPrve = _pocetak.GetSati() * 60 + _pocetak.GetMinute();
		int krajPrve = pocetakPrve + _trajanjeMinuta;

		int pocetakDruge = rezervacija.GetPocetak().GetSati() * 60 + rezervacija.GetPocetak().GetMinute();
		int krajDruge = pocetakDruge + rezervacija.GetTrajanjeMinuta();

		if (pocetakDruge > krajPrve || pocetakPrve > krajDruge) return false;
		return true;
	}
};
class PojedinacnaRezervacija : public Rezervacija {
public:
	PojedinacnaRezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta)
		: Rezervacija(pocetak, trajanjeMinuta) {
	}
	string Info() const override {
		stringstream ss;

		ss << setw(2) << setfill('0') << _pocetak.GetDan() << "."
			<< setw(2) << setfill('0') << _pocetak.GetMjesec() << "."
			<< setw(2) << setfill('0') << _pocetak.GetGodina() << " "
			<< setw(2) << setfill('0') << _pocetak.GetSati() << ":"
			<< setw(2) << setfill('0') << _pocetak.GetMinute() << " ";

		ss << "POJEDINACNA REZERVACIJA (" << _trajanjeMinuta << " min)";

		return ss.str();
	}
	Rezervacija* GetClone() const { return new PojedinacnaRezervacija(*this); };
};
class GrupnaRezervacija : public Rezervacija {
	int _brojUcesnika;
public:
	GrupnaRezervacija(const DatumVrijeme& pocetak, int trajanjeMinuta, int brojUcesnika)
		: Rezervacija(pocetak, trajanjeMinuta), _brojUcesnika(brojUcesnika) {
	}
	GrupnaRezervacija(const GrupnaRezervacija& obj) :Rezervacija(obj) {
		_brojUcesnika = obj.GetBrojUcesnika();
	}
	string Info() const override {
		stringstream ss;

		ss << setw(2) << setfill('0') << _pocetak.GetDan() << "."
			<< setw(2) << setfill('0') << _pocetak.GetMjesec() << "."
			<< setw(2) << setfill('0') << _pocetak.GetGodina() << " "
			<< setw(2) << setfill('0') << _pocetak.GetSati() << ":"
			<< setw(2) << setfill('0') << _pocetak.GetMinute() << " ";

		ss << "GRUPNA REZERVACIJA ZA " << _brojUcesnika << " OSOBA (" << _trajanjeMinuta << " min)";

		return ss.str();
	}
	int GetBrojUcesnika() const { return _brojUcesnika; };
	Rezervacija* GetClone() const { return new GrupnaRezervacija(*this); };
};
class Clan {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	char* _email;
	vector<Rezervacija*> _rezervacije;
public:
	Clan(string imePrezime, string email) {
		_sifra = AlocirajTekst(GenerisiSifruClana().c_str());
		_imePrezime = AlocirajTekst(imePrezime.c_str());
		_email = AlocirajTekst(email.c_str());
	}
	Clan(const Clan& obj) {
		_sifra = AlocirajTekst(obj.GetSifra());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_email = AlocirajTekst(obj.GetEmail());
		for (size_t i = 0; i < obj._rezervacije.size(); i++)
		{
			_rezervacije.push_back(obj._rezervacije[i]->GetClone());
		}
	}
	Clan& operator=(const Clan& obj) {
		if (this != &obj)
		{
			delete[] _imePrezime; delete[] _email; delete[] _sifra;
			for (auto* r : _rezervacije) delete r;
			_rezervacije.clear();

			_sifra = AlocirajTekst(obj.GetSifra());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_email = AlocirajTekst(obj.GetEmail());
			for (size_t i = 0; i < obj._rezervacije.size(); i++)
			{
				_rezervacije.push_back(obj._rezervacije[i]->GetClone());
			}
		}
		return *this;
	}
	~Clan() {
		delete[] _imePrezime; delete[] _email; delete[] _sifra;
		for (auto* r : _rezervacije) delete r;
		_rezervacije.clear();
	}
	const char* GetImePrezime() const { return _imePrezime; }
	const char* GetEmail() const { return _email; }
	const char* GetSifra() const { return _sifra; }
	vector<Rezervacija*>& GetRezervacije() { return _rezervacije; };
	// operator<< nije dat - format prikaza (clan + njegove rezervacije preko Info())
	// osmisliti samostalno.
	static string GenerisiSifruClana();
	bool operator==(const Clan& obj) const {
		return strcmp(_sifra, obj.GetSifra()) == 0;
	}
};
int Clan::_id = 1;
int getTrenutnuGodinu() {
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = (timeInfo.tm_year + 1900) % 100;
	return godina;
}
string Clan::GenerisiSifruClana() {
	stringstream ss;

	ss << "CL-"
		<< setw(2) << setfill('0') << getTrenutnuGodinu()
		<< setw(2) << setfill('0') << _id++;

	return ss.str();
}
class Prostorija {
	char* _oznaka;
	char* _naziv;
	TipProstorije _tip;
	int _kapacitet;
	double _cijenaPoSatu;
	vector<Rezervacija*> _rezervacije;
public:
	Prostorija(const char* naziv, TipProstorije tip, int kapacitet, double cijenaPoSatu, int sprat) {
		_naziv = AlocirajTekst(naziv);
		_tip = tip;
		_kapacitet = kapacitet;
		_cijenaPoSatu = cijenaPoSatu;

		_oznaka = AlocirajTekst(GenerisiOznakuProstorije(naziv, kapacitet, sprat).c_str());
	}
	Prostorija(const Prostorija& obj) {
		_oznaka = AlocirajTekst(obj.GetOznaka());
		_naziv = AlocirajTekst(obj.GetNaziv());
		_tip = obj.GetTip();
		_kapacitet = obj.GetKapacitet();
		_cijenaPoSatu = obj.GetCijenaPoSatu();
		for (size_t i = 0; i < obj._rezervacije.size(); i++)
		{
			_rezervacije.push_back(obj._rezervacije[i]->GetClone());
		}
	}
	Prostorija& operator=(const Prostorija& obj) {
		if (this != &obj)
		{
			delete[] _oznaka; delete[] _naziv;
			_oznaka = nullptr; _naziv = nullptr;

			_oznaka = AlocirajTekst(obj.GetOznaka());
			_naziv = AlocirajTekst(obj.GetNaziv());
			_tip = obj.GetTip();
			_kapacitet = obj.GetKapacitet();
			_cijenaPoSatu = obj.GetCijenaPoSatu();
			for (size_t i = 0; i < obj._rezervacije.size(); i++)
			{
				_rezervacije.push_back(obj._rezervacije[i]->GetClone());
			}
		}
		return *this;
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
	friend ostream& operator<<(ostream& COUT, const Prostorija& obj) {
		COUT << obj._oznaka << " | "
			<< obj._naziv << " | "
			<< obj._tip << " | "
			<< obj._kapacitet << " | "
			<< obj._cijenaPoSatu;

		return COUT;
	}
	bool operator==(const Prostorija& obj) const {
		return strcmp(_oznaka, obj.GetOznaka()) == 0;
	}
	vector<Rezervacija*>& GetRezervacije() { return _rezervacije; }
	double UkupanPrihod() {
		double zbir = 0.00;
		for (size_t i = 0; i < _rezervacije.size(); i++)
		{
			zbir += (_rezervacije[i]->GetTrajanjeMinuta() / 60.0) * _cijenaPoSatu;
		}
		return zbir;
	}
	int UkupnoZauzetihSati() {
		int ukupnoZauzetihMinuta = 0;
		for (size_t i = 0; i < _rezervacije.size(); i++)
		{
			ukupnoZauzetihMinuta += _rezervacije[i]->GetTrajanjeMinuta();
		}
		int ukupnoZauzetihSati = ukupnoZauzetihMinuta / 60;
		return ukupnoZauzetihSati;
	}
};
class UrbanHub {
	char* _naziv;
	vector<Clan> _clanovi;
	vector<Prostorija> _prostorije;
public:
	UrbanHub(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	UrbanHub(const UrbanHub& obj) {
		_naziv = AlocirajTekst(obj.GetNaziv());
		_clanovi = obj._clanovi;
		_prostorije = obj._prostorije;
	}
	UrbanHub& operator=(const UrbanHub& obj) {
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj.GetNaziv());
			_clanovi = obj._clanovi;
			_prostorije = obj._prostorije;
		}
		return *this;
	}
	~UrbanHub() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Clan>& GetClanovi() { return _clanovi; }
	vector<Prostorija>& GetProstorije() { return _prostorije; }
	const vector<Prostorija>& GetProstorije() const { return _prostorije; }
	void DodajProstoriju(const Prostorija& prostorija) {
		for (auto& p : _prostorije) {
			if (p == prostorija) throw exception("Prostorija sa istom oznakom vec postoji");
		}
		_prostorije.push_back(prostorija);
	}
	void DodajClana(const Clan& clan) {
		for (auto& c : _clanovi) {
			if (c == clan) throw exception("Clan je vec dodat");
		}
		_clanovi.push_back(clan);
	}
	bool RezervisiProstoriju(const char* sifraClana, string& oznaka, Rezervacija* rezervacija) {
		Clan* clan = nullptr;
		for (auto& c : _clanovi) {
			if (strcmp(c.GetSifra(), sifraClana) == 0) {
				clan = &c;
				break;
			}
		}
		Prostorija* prostorija = nullptr;
		for (auto& p : _prostorije) {
			if (strcmp(p.GetOznaka(), oznaka.c_str()) == 0) {
				prostorija = &p;
				break;
			}
		}
		if (clan == nullptr || prostorija == nullptr)
			return false;

		for (size_t i = 0; i < prostorija->GetRezervacije().size(); i++)
		{
			if (prostorija->GetRezervacije()[i]->DaLiImaKonflikt(*rezervacija)) return false;
		}

		if (GrupnaRezervacija* grupna = dynamic_cast<GrupnaRezervacija*>(rezervacija)) {
			int brojUcesnika = grupna->GetBrojUcesnika();
			int kapacitet = prostorija->GetKapacitet();
			if (brojUcesnika > kapacitet) return false;
		}

		prostorija->GetRezervacije().push_back(rezervacija);
		clan->GetRezervacije().push_back(rezervacija);

		if (GrupnaRezervacija* grupna = dynamic_cast<GrupnaRezervacija*>(rezervacija)) {
			int brojUcesnika = grupna->GetBrojUcesnika();
			if (brojUcesnika >= 5) {
				thread t([&]() {
					cout << crt;
					cout << "To: " << clan->GetEmail() << endl;
					cout << "From: rezervacije@urbanhub.ba" << endl;
					cout << "Subject: Potvrda grupne rezervacije" << endl;
					cout << "Postovani "<< clan->GetImePrezime() <<", \n Vasa grupna rezervacija za "<< brojUcesnika <<" osoba u prostoriji " << prostorija->GetOznaka() << " je potvrdjena." << endl;
					cout << "Hvala na povjerenju." << endl;
					cout << crt;
				});
				t.join();
			}
		}
		return true;
	}
	double UkupanPrihodHuba() {
		double ukupanPrihod = 0.00;
		for (size_t i = 0; i < _prostorije.size(); i++)
		{
			ukupanPrihod += _prostorije[i].UkupanPrihod();
		}
		return ukupanPrihod;
	}
	vector<Prostorija*> IzdvojiPreopterecene(int minSati) {
		vector<Prostorija*> temp;
		for (size_t i = 0; i < _prostorije.size(); i++)
		{
			int trajanjeUMinutama = 0;
			for (size_t j = 0; j < _prostorije[i].GetRezervacije().size(); j++)
			{
				Rezervacija* rezervacija = _prostorije[i].GetRezervacije()[j];
				trajanjeUMinutama += rezervacija->GetTrajanjeMinuta();
			}
			int trajanjeUSatima = trajanjeUMinutama / 60;
			if (trajanjeUSatima >= minSati) temp.push_back(&_prostorije[i]);
		}
		return temp;
	}
};

bool UcitajClanove(const string& putanja, vector<Clan>& clan) {
	fstream fajl{ putanja };

	if (!fajl.is_open()) return false;

	string imeClana;
	string mailClana;

	bool ucitano = false;

	while (getline(fajl, imeClana,'|') && getline(fajl, mailClana))
	{
		Clan* trenutniClan = nullptr;
		for (auto& c : clan) {
			if (c.GetImePrezime() == imeClana) {
				trenutniClan = &c;
				break;
			}
		}
		if (trenutniClan == nullptr) {
			clan.push_back(Clan(imeClana.c_str(), mailClana.c_str()));
			trenutniClan = &clan.back();
			ucitano = true;
		}
	}
	return ucitano;
}

int main() {
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
	DatumVrijeme sastanak2(3, 5, 2026, 9, 30);
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

	/*DodajProstoriju dodaje prostoriju u hub.Ne dozvoliti dvije prostorije sa
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

	//DodajClana dodaje clana u hub. Ne dozvoliti dva clana sa istom sifrom -
	//baciti izuzetak.
	Clan amina("Amina Buric", "amina@mail.com");
	Clan goran("Goran Skondric", "goran@mail.com");
	hub.DodajClana(amina);
	hub.DodajClana(goran);

	string oznakaFeniksa = hub.GetProstorije()[0].GetOznaka();

	/*RezervisiProstoriju pronalazi clana (po sifri) i prostoriju (po oznaci) i
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
	Metoda vraca true ako je rezervacija uspjesno kreirana, u suprotnom false.*/

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

	/*IzdvojiPreopterecene vraca pokazivace na sve prostorije cija je ukupna
	zauzetost (u satima, preko svih rezervacija) veca ili jednaka
	proslijedjenoj vrijednosti minSati.*/
	vector<Prostorija*> preopterecene = hub.IzdvojiPreopterecene(2);
	for (auto* p : preopterecene)
		cout << p->GetOznaka() << " -> " << p->UkupnoZauzetihSati() << "h zauzetosti" << crt;

	/*Funkcija UcitajClanove ucitava clanove iz fajla cije ime se proslijedjuje
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