#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <regex>
#include <thread>
#include <functional>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <fstream>
using namespace std;
char* AlocirajTekst(const char* tekst) {
	if (tekst == nullptr) return nullptr;
	size_t velicina = strlen(tekst) + 1;
	char* noviTekst = new char[velicina];
	strcpy_s(noviTekst, velicina, tekst);
	return noviTekst;
}
enum TipPlovila { JEDRILICA, MOTORNI_CAMAC, KATAMARAN, JAHTA };
const char* TipPlovilaNazivi[] = {
"JEDRILICA", "MOTORNI CAMAC", "KATAMARAN", "JAHTA" };
const char* crt = "\n---------------------------------------------------------------------------------\n";
string GenerisiOznakuPlovila(const char* naziv, int kapacitet, int godinaProizvodnje) {
	if (kapacitet < 1 || kapacitet > 200) return "MR-000/XX-X";
	if (naziv == nullptr || strlen(naziv) < 1)  return "MR-000/XX-X";

	string oznaka;
	oznaka += "MR-";

	// KKK
	if (kapacitet < 10) oznaka += "00";
	else if (kapacitet < 100) oznaka += "0";
	oznaka += to_string(kapacitet);
	oznaka += "/";

	// VV
	oznaka += toupper(naziv[0]);
	const char* razmak = strchr(naziv, ' ');
	if (razmak != nullptr && strlen(razmak) > 1) oznaka += toupper(razmak[1]);
	if (razmak == nullptr)  return "MR-000/XX-X";
	oznaka += "-";

	// G
	int zadnjaCifraGodineProizvodnje = godinaProizvodnje % 10;
	oznaka += to_string(zadnjaCifraGodineProizvodnje);

	return oznaka;
}
bool ValidirajOznakuPlovila(const string& oznaka) {
	return regex_match(oznaka, regex("MR-(?!000)\\d{3}/[A-Z]{2}-\\d"));
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
			COUT << obj.GetElement1(i) << " " << obj.GetElement2(i) << endl;
		return COUT;
	}
	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max) throw exception("Kolekcija je puna");

		_elementi1[_trenutno] = el1;
		_elementi2[_trenutno] = el2;

		_trenutno++;
	}
	Kolekcija IzdvojiUOpsegu(const T2& donjaGranica, const T2& gornjaGranica) {
		Kolekcija temp;
		for (size_t i = 0; i < _trenutno; i++)
		{
			if (_elementi2[i] >= donjaGranica && _elementi2[i] <= gornjaGranica) temp.Dodaj(GetElement1(i), GetElement2(i));
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
	friend ostream& operator<<(ostream& COUT, const DatumVrijeme& obj) {
		COUT << obj.ToString();
		return COUT;
	}
	string ToString() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << *_dan << "."
			<< setw(2) << setfill('0') << *_mjesec << "."
			<< setw(2) << setfill('0') << *_godina << " "
			<< setw(2) << setfill('0') << *_sati << ":"
			<< setw(2) << setfill('0') << *_minute;

		return ss.str();
	};
	bool poklapaLiSeDatum(const DatumVrijeme& obj) const {
		return *_dan == obj.GetDan() &&
			*_mjesec == obj.GetMjesec() &&
			*_godina == obj.GetGodina();
	}
};
class Najam {
protected:
	DatumVrijeme _pocetak;
	int _trajanjeSati;
public:
	Najam(const DatumVrijeme& pocetak, int trajanjeSati)
		: _pocetak(pocetak), _trajanjeSati(trajanjeSati) {
	}
	Najam(const Najam& obj) {
		_pocetak = obj.GetPocetak();
		_trajanjeSati = obj.GetTrajanjeSati();
	}
	Najam& operator=(const Najam& obj) {
		if (this != &obj)
		{
			_pocetak = obj.GetPocetak();
			_trajanjeSati = obj.GetTrajanjeSati();
		}
		return *this;
	}
	virtual ~Najam() {}
	virtual string Info() const = 0;
	virtual Najam* GetClone() const = 0;
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeSati() const { return _trajanjeSati; }
	bool daLiSeTerminiPoklapaju(const Najam& najam) {
		int pocetakPrvog = _pocetak.GetMinute() + _pocetak.GetSati() * 60;
		int krajPrvog = pocetakPrvog + _trajanjeSati * 60;

		int pocetakDrugog = najam.GetPocetak().GetMinute() + najam.GetPocetak().GetSati() * 60;
		int krajDrugog = pocetakDrugog + najam.GetTrajanjeSati() * 60;

		if (pocetakDrugog >= krajPrvog || pocetakPrvog >= krajDrugog) return false;
		return true;
	}
};
class IndividualniNajam : public Najam {
public:
	IndividualniNajam(const DatumVrijeme& pocetak, int trajanjeSati)
		: Najam(pocetak, trajanjeSati) {
	}
	// Info() nije preklopljena - dodati override koji vraca npr:
	// "03.05.2026 09:00 INDIVIDUALNI NAJAM (3h)"
	~IndividualniNajam() {};
	string Info() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << _pocetak.GetDan() << "."
			<< setw(2) << setfill('0') << _pocetak.GetMjesec() << "."
			<< setw(2) << setfill('0') << _pocetak.GetGodina() << " "
			<< setw(2) << setfill('0') << _pocetak.GetSati() << ":"
			<< setw(2) << setfill('0') << _pocetak.GetMinute();

		ss << "INDIVIDUALNI NAJAM (" << GetTrajanjeSati() << "h)";
		return ss.str();
	}
	Najam* GetClone() const { return new IndividualniNajam(*this); };
};
class GrupniNajam : public Najam {
	int _brojPutnika;
public:
	GrupniNajam(const DatumVrijeme& pocetak, int trajanjeSati, int brojPutnika)
		: Najam(pocetak, trajanjeSati), _brojPutnika(brojPutnika) {
	}
	~GrupniNajam() {};
	string Info() const {
		stringstream ss;

		ss << setw(2) << setfill('0') << _pocetak.GetDan() << "."
			<< setw(2) << setfill('0') << _pocetak.GetMjesec() << "."
			<< setw(2) << setfill('0') << _pocetak.GetGodina() << " "
			<< setw(2) << setfill('0') << _pocetak.GetSati() << ":"
			<< setw(2) << setfill('0') << _pocetak.GetMinute();

		ss << "GRUPNI NAJAM ZA " << GetBrojPutnika() << " OSOBA (" << GetTrajanjeSati() << "h)";
		return ss.str();
	}
	int GetBrojPutnika()const { return _brojPutnika; };
	Najam* GetClone() const { return new GrupniNajam(*this); };
};
class Klijent {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	char* _brojTelefona;
	vector<Najam*> _najmovi;
public:
	Klijent(string imePrezime, string brojTelefona) {
		_imePrezime = AlocirajTekst(imePrezime.c_str());
		_brojTelefona = AlocirajTekst(brojTelefona.c_str());
		_sifra = AlocirajTekst(GenerisiSifruKlijenta().c_str());
	}
	Klijent(const Klijent& obj) {
		_sifra = AlocirajTekst(obj.GetSifra());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_brojTelefona = AlocirajTekst(obj.GetBrojTelefona());
		for (size_t i = 0; i < obj._najmovi.size(); i++)
		{
			_najmovi.push_back(obj._najmovi[i]->GetClone());
		}
	}
	Klijent& operator=(const Klijent& obj) {
		if (this != &obj)
		{
			delete[] _imePrezime; delete[] _brojTelefona; delete[] _sifra;
			for (auto* n : _najmovi) delete n;
			_najmovi.clear();

			_sifra = AlocirajTekst(obj.GetSifra());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_brojTelefona = AlocirajTekst(obj.GetBrojTelefona());
			for (size_t i = 0; i < obj._najmovi.size(); i++)
			{
				_najmovi.push_back(obj._najmovi[i]->GetClone());
			}
		}
		return *this;
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
	static string GenerisiSifruKlijenta();
	bool operator==(const Klijent& obj) const {
		return strcmp(_sifra, obj.GetSifra()) == 0;
	}
	vector<Najam*>& GetNajmovi() { return _najmovi; };
};
int Klijent::_id = 1;
class Plovilo {
	char* _oznaka;
	char* _naziv;
	TipPlovila _tip;
	int _kapacitet;
	double _cijenaPoSatu;
	vector<Najam*> _najmovi;
public:
	Plovilo(const char* naziv, TipPlovila tip, int kapacitet, double cijenaPoSatu, int godinaProizvodnje) {
		_naziv = AlocirajTekst(naziv);
		_tip = tip;
		_kapacitet = kapacitet;
		_cijenaPoSatu = cijenaPoSatu;
		_oznaka = AlocirajTekst(GenerisiOznakuPlovila(naziv, kapacitet, godinaProizvodnje).c_str());
	}
	Plovilo(const Plovilo& obj) {
		_oznaka = AlocirajTekst(obj.GetOznaka());
		_naziv = AlocirajTekst(obj.GetNaziv());
		_tip = obj.GetTip();
		_kapacitet = obj.GetKapacitet();
		_cijenaPoSatu = obj.GetCijenaPoSatu();
		for (size_t i = 0; i < obj._najmovi.size(); i++)
		{
			_najmovi.push_back(obj._najmovi[i]->GetClone());
		}
	}
	Plovilo& operator=(const Plovilo& obj) {
		if (this != &obj)
		{
			delete[] _oznaka; delete[] _naziv;
			_oznaka = nullptr; _naziv = nullptr;

			_oznaka = AlocirajTekst(obj.GetOznaka());
			_naziv = AlocirajTekst(obj.GetNaziv());
			_tip = obj.GetTip();
			_kapacitet = obj.GetKapacitet();
			_cijenaPoSatu = obj.GetCijenaPoSatu();
			for (size_t i = 0; i < obj._najmovi.size(); i++)
			{
				_najmovi.push_back(obj._najmovi[i]->GetClone());
			}
		}
		return *this;
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
	friend ostream& operator<<(ostream& COUT, const Plovilo& obj) {
		COUT << "Oznaka: " << obj.GetOznaka() << " :: " << endl;
		COUT << "Naziv: " << obj.GetNaziv() << " :: " << endl;
		COUT << "Tip: " << TipPlovilaNazivi[obj.GetTip()] << " :: " << endl;
		COUT << "Kapacitet: " << obj.GetKapacitet() << " :: " << endl;
		COUT << "Cijena po satu: " << obj.GetKapacitet() << "KM";
		return COUT;
	}
	bool operator==(const Plovilo& obj) const {
		return strcmp(_oznaka, obj.GetOznaka()) == 0;
	}
	vector<Najam*>& GetNajmovi() { return _najmovi; };
	double UkupanPrihod() {
		double ukupanPrihod = 0.00;
		for (size_t i = 0; i < _najmovi.size(); i++)
		{
			ukupanPrihod += _cijenaPoSatu * _najmovi[i]->GetTrajanjeSati();
		}
		return ukupanPrihod;
	}
	double UkupnoZauzetihSati() {
		double zauzetost = 0.00;
		for (size_t i = 0; i < _najmovi.size(); i++)
		{
			zauzetost += _najmovi[i]->GetTrajanjeSati();
		}
		return zauzetost;
	}
};
int getTrenutnuGodinuIzSistema() {
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = (timeInfo.tm_year + 1900) % 100;
	return godina;
}
string Klijent::GenerisiSifruKlijenta() {
	stringstream ss;

	ss << "KL-" << setw(2) << setfill('0') << getTrenutnuGodinuIzSistema() << "-" << setw(3) << setfill('0') << _id++;
	return ss.str();
}
class Marina {
	char* _naziv;
	vector<Klijent> _klijenti;
	vector<Plovilo> _plovila;
public:
	Marina(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	Marina(const Marina& obj) {
		_naziv = AlocirajTekst(obj.GetNaziv());
		_klijenti = obj._klijenti;
		_plovila = obj._plovila;
	}
	Marina& operator=(const Marina& obj) {
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj.GetNaziv());
			_klijenti = obj._klijenti;
			_plovila = obj._plovila;
		}
		return *this;
	}
	~Marina() { delete[] _naziv; _naziv = nullptr; }
	const char* GetNaziv() const { return _naziv; }
	vector<Klijent>& GetKlijenti() { return _klijenti; }
	vector<Plovilo>& GetPlovila() { return _plovila; }
	void DodajPlovilo(const Plovilo& plovilo) {
		for (auto& p : _plovila) {
			if (p == plovilo) throw exception("Plovilo sa datom oznakom vec postoji");
		}
		_plovila.push_back(plovilo);
	}
	void DodajKlijenta(const Klijent& klijent) {
		for (auto& k : _klijenti) {
			if (k == klijent) throw exception("Klijent sa datom sifrom vec postoji");
		}
		_klijenti.push_back(klijent);
	}
	bool NajmiPlovilo(const char* sifra, string oznaka, Najam* najam) {
		Klijent* klijent = nullptr;
		for (auto& k : _klijenti) {
			if (strcmp(k.GetSifra(), sifra) == 0) {
				klijent = &k;
				break;
			}
		}

		Plovilo* plovilo = nullptr;
		for (auto& p : _plovila) {
			if (strcmp(p.GetOznaka(), oznaka.c_str()) == 0) {
				plovilo = &p;
				break;
			}
		}

		if (klijent == nullptr || plovilo == nullptr) return false;

		for (size_t i = 0; i < plovilo->GetNajmovi().size(); i++)
		{
			if (!plovilo->GetNajmovi()[i]->GetPocetak().poklapaLiSeDatum(najam->GetPocetak())) continue;
			if (plovilo->GetNajmovi()[i]->GetPocetak().poklapaLiSeDatum(najam->GetPocetak())) {
				if (plovilo->GetNajmovi()[i]->daLiSeTerminiPoklapaju(*najam)) return false;
			}
		}

		GrupniNajam* grupniNajam = dynamic_cast<GrupniNajam*>(najam);
		if (grupniNajam != nullptr)
		{
			if (grupniNajam->GetBrojPutnika() > plovilo->GetKapacitet()) return false;
		}

		plovilo->GetNajmovi().push_back(najam->GetClone());
		klijent->GetNajmovi().push_back(najam->GetClone());

		if (grupniNajam != nullptr && grupniNajam->GetBrojPutnika() >= 5) {
			thread t([&]() {
				cout << "---------------------------------------------------------------------------" << endl;
				cout << "Broj: " << klijent->GetBrojTelefona() << endl;
				cout << "Od: " << _naziv << endl;
				cout << "Poruka: Postovani " << klijent->GetImePrezime() << ", Vasa grupna voznja za " << grupniNajam->GetBrojPutnika() << " osoba na plovilu\n";
				cout << plovilo->GetOznaka() << " je potvrdjena. Hvala na povjerenju." << endl;
				cout << "---------------------------------------------------------------------------" << endl;
				});
			t.join();
		}
		return true;
	}
	double UkupanPrihodMarine() {			
		double ukupanPrihod = 0.00;
		for (size_t i = 0; i < _plovila.size(); i++)
		{
			ukupanPrihod += _plovila[i].UkupanPrihod();
		}
		return ukupanPrihod;
	}
	vector<Plovilo*> IzdvojiPreopterecena(int minSati) {
		vector<Plovilo*> temp;
		for (size_t i = 0; i < _plovila.size(); i++)
		{
			if (_plovila[i].UkupnoZauzetihSati() >= minSati) temp.push_back(&_plovila[i]);
		}
		return temp;
	}
	vector<Plovilo*> IzdvojiPoUslovu(function<bool(const Plovilo&)> uslov) {
		vector<Plovilo*> temp;
		for (size_t i = 0; i < _plovila.size(); i++)
		{
			if (uslov(_plovila[i]) == true) temp.push_back(&_plovila[i]);
		}
		return temp;
	}
};

bool UcitajKlijente(const string& putanja, vector<Klijent>& klijenti) {
	fstream fajl{ putanja };

	if (!fajl.is_open()) return false;

	string imeKlijenta, brojTelefona;

	bool ucitan = false;

	while (getline(fajl, imeKlijenta, '|') && getline(fajl, brojTelefona))
	{
		bool pronadjen = false;

		Klijent* trenutniKlijent = nullptr;
		for (auto& k : klijenti) {
			if (k.GetImePrezime() == imeKlijenta) {
				trenutniKlijent = &k;
				pronadjen = true;
				break;
			}
		}
		if (!pronadjen)
		{
			klijenti.push_back(Klijent(imeKlijenta, brojTelefona));
			trenutniKlijent = &klijenti.back();
			ucitan = true;
		}
	}
	return ucitan;
}

int main() {

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
	DatumVrijeme voznja2(3, 7, 2026, 9, 30);
	DatumVrijeme voznja3(3, 7, 2026, 13, 0);

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