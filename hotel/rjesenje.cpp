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
	if (tekst == nullptr) return nullptr;
	size_t velicina = strlen(tekst) + 1;
	char* noviTekst = new char[velicina];
	strcpy_s(noviTekst, velicina, tekst);
	return noviTekst;
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
// Za imena sa srednjim imenom, uzima se prvo slovo imena i prvo slovo prezimena (NE srednje ime)
// Primjer: "Ana Marija Kovac" -> AK (A od Ana, K od Kovac)
// BB - tip sobe (ST, AP, LK, SD)
// 0003 - redni broj popunjen nulama na 4 mjesta
// 2026 - trenutna godina
// Za neispravne podatke vraca "HRS-XX-XX-0000/0000"
string GenerisiIDRezervacije(const char* imePrezime, int redniBroj, TipSobe tipSobe) {
	if (imePrezime == nullptr || strlen(imePrezime) == 0) return "HRS-XX-XX-0000/0000";
	if (redniBroj < 1 || redniBroj > 9999)  return "HRS-XX-XX-0000/0000";
	string id;

	// HRS
	id += "HRS-";

	// IN
	id += toupper(imePrezime[0]);
	const char* zadnjiRazmak = strrchr(imePrezime, ' ');
	if (zadnjiRazmak != nullptr && strlen(zadnjiRazmak) > 1) {
		id += toupper(zadnjiRazmak[1]);
	}
	else {
		return "HRS-XX-XX-0000/0000";
	}
	id += "-";

	// BB
	switch (tipSobe) {
	case STANDARDNA: id += "ST"; break;
	case APARTMAN: id += "AP"; break;
	case LUKSUZ: id += "LK"; break;
	case STUDIO: id += "SD"; break;
	default: return "HRS-XX-XX-0000/0000";
	}
	id += "-";

	// XXXX
	if (redniBroj < 10) id += "000";
	else if (redniBroj < 100) id += "00";
	else if (redniBroj < 1000) id += "0";
	id += to_string(redniBroj);
	id += "/";

	// GGGG
	int godina = getTrenutnoGodinuIzSistema();
	id += to_string(godina);
	return id;
};

bool ValidirajIDRezervacije(const string& id) {
	return regex_match(id, regex("HRS-[A-Z]{2}-(ST|AP|LK|SD)-(?!0000)\\d{4}/2026"));
}

template<class T1, class T2, int max>
class Kolekcija {
	T1* _prvi;
	T2* _drugi;
	int _trenutno;
public:
	Kolekcija() : _prvi(nullptr), _drugi(nullptr), _trenutno(0) {}

	Kolekcija(const Kolekcija& obj) {
		_trenutno = obj.GetTrenutno();
		_prvi = new T1[_trenutno];
		_drugi = new T2[_trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_prvi[i] = obj._prvi[i];
			_drugi[i] = obj._drugi[i];
		}
	};

	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete[] _prvi;
			delete[] _drugi;

			_prvi = nullptr;
			_drugi = nullptr;

			_trenutno = obj.GetTrenutno();
			_prvi = new T1[_trenutno];
			_drugi = new T2[_trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_prvi[i] = obj._prvi[i];
				_drugi[i] = obj._drugi[i];
			}
		}
		return *this;
	};

	~Kolekcija() {
		delete[] _prvi;
		delete[] _drugi;

		_prvi = nullptr;
		_drugi = nullptr;
	};

	int GetTrenutno() const { return _trenutno; };
	T1& GetPrvi(int indeks) { return _prvi[indeks]; };
	T2& GetDrugi(int indeks) { return _drugi[indeks]; };
	const T1& GetPrvi(int indeks) const { return _prvi[indeks]; };
	const T2& GetDrugi(int indeks) const { return _drugi[indeks]; };
	T1& operator[](int indeks) { return _prvi[indeks]; };

	friend ostream& operator<<(ostream& COUT, const Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetPrvi(i) << " " << obj.GetDrugi(i) << endl;
		return COUT;
	};

	// Dodaje novi par na kraj kolekcije. Ako je kolekcija puna, baca izuzetak.
	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max) return;

		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _prvi[i];
			temp2[i] = _drugi[i];
		}

		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;

		delete[] _prvi;
		delete[] _drugi;

		_prvi = temp1;
		_drugi = temp2;

		_trenutno++;
	};

	// Dodaje novi par na poziciju, pomjera postojece udesno i vraca trenutno stanje kolekcije.
	// U slucaju popunjene kolekcije ili neispravne lokacije baca izuzetak.
	Kolekcija<T1, T2, max> DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2) {
		if (pozicija < 0 || pozicija > _trenutno) throw exception("Pozicija nije ispravna");
		if (_trenutno >= max) throw exception("Kolekcija puna!");

		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];

		for (size_t i = 0; i < pozicija; i++)
		{
			temp1[i] = _prvi[i];
			temp2[i] = _drugi[i];
		}

		temp1[pozicija] = el1;
		temp2[pozicija] = el2;

		for (size_t i = pozicija; i < _trenutno; i++)
		{
			temp1[i + 1] = _prvi[i];
			temp2[i + 1] = _drugi[i];
		}

		delete[] _prvi;
		delete[] _drugi;

		_prvi = temp1;
		_drugi = temp2;

		_trenutno++;
		return *this;
	};

	// Uklanja raspon elemenata od start do start+brojElemenata-1.
	// Vraca pokazivac na novu kolekciju sa uklonjenim elementima.
	// Za neispravan raspon baca izuzetak.
	Kolekcija<T1, T2, max>* UkloniRaspon(int start, int brojElemenata) {
		if (start < 0 || start + brojElemenata > _trenutno || start > _trenutno)
			throw exception("Opseg nije validan!");

		Kolekcija<T1, T2, max>* uklonjeni = new Kolekcija<T1, T2, max>();

		for (int i = start; i < start + brojElemenata; i++) {
			uklonjeni->Dodaj(_prvi[i], _drugi[i]);
		}

		for (int i = start + brojElemenata; i < _trenutno; i++) {
			_prvi[i - brojElemenata] = _prvi[i];
			_drugi[i - brojElemenata] = _drugi[i];
		}

		_trenutno -= brojElemenata;
		return uklonjeni;
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
	};

	DatumVrijeme(const DatumVrijeme& obj) {
		_dan = new int(*obj._dan);
		_mjesec = new int(*obj._mjesec);
		_godina = new int(*obj._godina);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
	};

	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
			_dan = _mjesec = _godina = _sati = _minute = nullptr;

			_dan = new int(*obj._dan);
			_mjesec = new int(*obj._mjesec);
			_godina = new int(*obj._godina);
			_sati = new int(*obj._sati);
			_minute = new int(*obj._minute);
		}
		return *this;
	};

	~DatumVrijeme() {
		delete _dan; delete _mjesec; delete _godina; delete _sati; delete _minute;
		_dan = _mjesec = _godina = _sati = _minute = nullptr;
	};

	int getDan() const { return *_dan; };
	int getMjesec() const { return *_mjesec; };
	int getGodina() const { return *_godina; };
	int getSati() const { return *_sati; };
	int getMinute() const { return *_minute; };

	// Vraca datum u formatu: DD.MM.GGGG HH:MM (05.10.2026 14:30)
	string ToString() const {
		stringstream ss;
		ss << setw(2) << setfill('0') << *_dan << "."
			<< setw(2) << setfill('0') << *_mjesec << "."
			<< setw(2) << setfill('0') << *_godina << " "
			<< setw(2) << setfill('0') << *_sati << ":"
			<< setw(2) << setfill('0') << *_minute;
		return ss.str();
	};

	bool operator>(const DatumVrijeme& obj) const {
		if (*_godina > obj.getGodina()) return true;
		if (*_godina < obj.getGodina()) return false;

		if (*_mjesec > obj.getMjesec()) return true;
		if (*_mjesec < obj.getMjesec()) return false;

		if (*_dan > obj.getDan()) return true;
		if (*_dan < obj.getDan()) return false;

		if (*_sati > obj.getSati()) return true;
		if (*_sati < obj.getSati()) return false;

		if (*_minute > obj.getMinute()) return true;
		if (*_minute < obj.getMinute()) return false;

		return false;
	};
	//bool operator==(const DatumVrijeme& obj) const;
};

class Osoba {
protected:
	char* _imePrezime;
	char* _email;
	char* _brojTelefona;
public:
	Osoba(const char* imePrezime = "", const char* email = "", const char* brojTelefona = "") {
		_imePrezime = AlocirajTekst(imePrezime);
		_email = AlocirajTekst(email);
		_brojTelefona = AlocirajTekst(brojTelefona);
	};

	Osoba(const Osoba& obj) {
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_email = AlocirajTekst(obj.GetEmail());
		_brojTelefona = AlocirajTekst(obj.GetBrojTelefona());
	};

	Osoba& operator=(const Osoba& obj) {
		if (this != &obj)
		{
			delete[] _imePrezime;
			delete[] _email;
			delete[] _brojTelefona;

			_imePrezime = _email = _brojTelefona = nullptr;

			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_email = AlocirajTekst(obj.GetEmail());
			_brojTelefona = AlocirajTekst(obj.GetBrojTelefona());
		}
		return *this;
	};

	virtual ~Osoba() {
		delete[] _imePrezime;
		delete[] _email;
		delete[] _brojTelefona;

		_imePrezime = _email = _brojTelefona = nullptr;
	};

	const char* GetImePrezime() const { return _imePrezime; };
	const char* GetEmail() const { return _email; };
	const char* GetBrojTelefona() const { return _brojTelefona; };

	virtual string ToString() const = 0;

	bool operator==(const Osoba& obj) const {
		return strcmp(_imePrezime, obj.GetImePrezime()) == 0;
	};
};

class Gost : public Osoba {
	TipGosta _tipGosta;
	int _brojPosjeta;
	double _ukupnaPotrosnja;
public:
	Gost(const char* imePrezime = "", const char* email = "", const char* brojTelefona = "",
		TipGosta tipGosta = REGULARNI, int brojPosjeta = 0, double ukupnaPotrosnja = 0.0) :Osoba(imePrezime, email, brojTelefona) {
		_tipGosta = tipGosta;
		_brojPosjeta = brojPosjeta;
		_ukupnaPotrosnja = ukupnaPotrosnja;
	};

	Gost(const Gost& obj) : Osoba(obj) {
		_tipGosta = obj.GetTipGosta();
		_brojPosjeta = obj.GetBrojPosjeta();
		_ukupnaPotrosnja = obj.GetUkupnaPotrosnja();
	};

	Gost& operator=(const Gost& obj) {
		if (this != &obj)
		{
			Osoba::operator=(obj);

			_tipGosta = obj.GetTipGosta();
			_brojPosjeta = obj.GetBrojPosjeta();
			_ukupnaPotrosnja = obj.GetUkupnaPotrosnja();
		}
		return *this;
	};

	~Gost() {};

	TipGosta GetTipGosta() const { return _tipGosta; };
	int GetBrojPosjeta() const { return _brojPosjeta; };
	double GetUkupnaPotrosnja() const { return _ukupnaPotrosnja; };
	//void DodajPosjetu(double iznos);

	// Override ToString - vraca: Ime Prezime | email | TIP | X posjeta | YYYY.ZZ KM
	string ToString() const {
		stringstream ss;
		ss << _imePrezime << " | " << _email << " | " << TipGostaNazivi[_tipGosta] << " | " << _brojPosjeta << " posjeta | " << _ukupnaPotrosnja << " KM ";
		return ss.str();
	};

	// Racuna popust: VIP 20%, POSLOVNI 15%, PORODICA 10%, REGULARNI 0%
	// Dodatni popust na osnovu broja posjeta
	double IzracunajPopust() const {
		double popust = 0.0;
		
		if (_tipGosta == VIP) popust = 0.20;         
		else if (_tipGosta == POSLOVNI) popust = 0.15;
		else if (_tipGosta == PORODICA) popust = 0.10;
		else popust = 0.00;

		if (_tipGosta == VIP) {
			double dodatniPopust = _brojPosjeta / 5;
			if (dodatniPopust > 10) dodatniPopust = 10;
			popust += dodatniPopust / 100.0;
		}
		else if (_tipGosta == POSLOVNI) {
			double dodatniPopust = _brojPosjeta / 5;
			if (dodatniPopust > 5) dodatniPopust = 5;
			popust += dodatniPopust / 200.0;
		}
		else if (_tipGosta == PORODICA) {
			double dodatniPopust = _brojPosjeta / 3;
			if (dodatniPopust > 5) dodatniPopust = 5;
			popust += dodatniPopust / 100.0;
		}
		else if (_tipGosta == REGULARNI) {
			double dodatniPopust = _brojPosjeta / 10;
			if (dodatniPopust > 5) dodatniPopust = 5;
			popust += dodatniPopust / 200.0;
		}

		return popust;
	};
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
		DatumVrijeme datumOdjave, int brojGostiju, int redniBroj)
		: _gost(gost), _tipSobe(tipSobe), _datumPrijave(datumPrijave),
		_datumOdjave(datumOdjave), _brojGostiju(brojGostiju), _status(KREIRANA) {

		_idRezervacije = AlocirajTekst(GenerisiIDRezervacije(_gost->GetImePrezime(), redniBroj, tipSobe).c_str());
		_napomena = AlocirajTekst("");
	}
	Rezervacija(const Rezervacija& obj)
		: _gost(obj._gost), _tipSobe(obj._tipSobe),
		_datumPrijave(obj._datumPrijave), _datumOdjave(obj._datumOdjave),
		_brojGostiju(obj._brojGostiju), _status(obj._status) {

		_idRezervacije = AlocirajTekst(obj._idRezervacije);
		_napomena = AlocirajTekst(obj._napomena);
		_dodatneUsluge = obj._dodatneUsluge;
	}
	Rezervacija& operator=(const Rezervacija& obj) {
		if (this != &obj) {
			delete[] _idRezervacije;
			delete[] _napomena;
			_idRezervacije = nullptr;
			_napomena = nullptr;

			_idRezervacije = AlocirajTekst(obj._idRezervacije);
			_napomena = AlocirajTekst(obj._napomena);
			_gost = obj._gost;
			_tipSobe = obj._tipSobe;
			_datumPrijave = obj._datumPrijave;
			_datumOdjave = obj._datumOdjave;
			_brojGostiju = obj._brojGostiju;
			_dodatneUsluge = obj._dodatneUsluge;
			_status = obj._status;
		}
		return *this;
	}
	~Rezervacija() {
		delete[] _idRezervacije;
		delete[] _napomena;
		_idRezervacije = nullptr;
		_napomena = nullptr;
	}
	const char* GetIdRezervacije() const {
		return _idRezervacije;
	}

	const char* GetNapomena() const {
		return _napomena;
	}

	Gost* GetGost() const {
		return _gost;
	}

	TipSobe GetTipSobe() const {
		return _tipSobe;
	}

	DatumVrijeme& GetDatumPrijave() {
		return _datumPrijave;
	}

	const DatumVrijeme& GetDatumPrijave() const {
		return _datumPrijave;
	}

	DatumVrijeme& GetDatumOdjave() {
		return _datumOdjave;
	}

	const DatumVrijeme& GetDatumOdjave() const {
		return _datumOdjave;
	}

	int GetBrojGostiju() const {
		return _brojGostiju;
	}

	Kolekcija<string, double, 10>& GetDodatneUsluge() {
		return _dodatneUsluge;
	}

	StatusRezervacije GetStatus() const {
		return _status;
	}

	string ToString() const {
		stringstream ss;
		ss << _idRezervacije << " | "
			<< _gost->GetImePrezime() << " | "
			<< TipSobeNazivi[_tipSobe] << " | "
			<< _datumPrijave.ToString() << " - "
			<< _datumOdjave.ToString() << " | "
			<< _brojGostiju << " gosta | "
			<< StatusRezervacijeNazivi[_status];
		return ss.str();
	}
	friend ostream& operator<<(ostream& COUT, const Rezervacija& obj) {
		COUT << obj.ToString();
		return COUT;
	}

	bool DodajDodatnuUslugu(const string& nazivUsluge, double cijena) {
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			if (_dodatneUsluge.GetPrvi(i) == nazivUsluge) return false;
		}
		_dodatneUsluge.Dodaj(nazivUsluge, cijena);
		return true;
	}

	double IzracunajUkupnuCijenu() {
		double cijenaPoNoci = 0;
		switch (_tipSobe)
		{
		case STANDARDNA: cijenaPoNoci += 100.0; break;
		case APARTMAN: cijenaPoNoci += 200.0; break;
		case LUKSUZ: cijenaPoNoci += 350.0; break;
		case STUDIO:cijenaPoNoci += 150.0; break;
		}

		int brojNocenja = _datumOdjave.getDan() - _datumPrijave.getDan();

		double cijenaSobe = cijenaPoNoci * brojNocenja;

		double dodatneUsluge = 0;
		for (size_t i = 0; i < _dodatneUsluge.GetTrenutno(); i++)
		{
			dodatneUsluge += _dodatneUsluge.GetDrugi(i);
		}

		double podUkupno = cijenaSobe + dodatneUsluge;
		double popust = _gost->IzracunajPopust();

		return podUkupno * (1 - popust);
	}

	bool PromijeniStatus(StatusRezervacije noviStatus) {
		if (_status == ZAVRSENA || _status == ODBIJENA) return false;
		if (_status == noviStatus) {
			return false;
		}
		if (_status == KREIRANA) {
			if (noviStatus == POTVRDJENA || noviStatus == ODBIJENA) {
				_status = noviStatus;
				return true;  
			}
		}
		else if (_status == POTVRDJENA) {
			if (noviStatus == OTPLACENA || noviStatus == ODBIJENA) {
				_status = noviStatus;
				return true;  
			}
		}
		else if (_status == OTPLACENA) {
			if (noviStatus == ZAVRSENA) {
				_status = noviStatus;
				return true;  
			}
		}
		return false;
	}
};

class Hotel {
	char* _naziv;
	char* _adresa;
	vector<Gost> _gosti;
	vector<Rezervacija> _rezervacije;
public:
	Hotel(const char* naziv = "", const char* adresa = "") {
		_naziv = AlocirajTekst(naziv);
		_adresa = AlocirajTekst(adresa);
	};

	Hotel(const Hotel& obj) {
		_naziv = AlocirajTekst(obj._naziv);
		_adresa = AlocirajTekst(obj._adresa);
		_gosti = obj._gosti;
		_rezervacije = obj._rezervacije;
	};

	Hotel& operator=(const Hotel& obj) {
		if (this != &obj)
		{
			delete[] _naziv;
			delete[] _adresa;
			_naziv = _adresa = nullptr;

			_naziv = AlocirajTekst(obj._naziv);
			_adresa = AlocirajTekst(obj._adresa);
			_gosti = obj._gosti;
			_rezervacije = obj._rezervacije;
		}
		return *this;
	};

	~Hotel() {
		delete[] _naziv;
		delete[] _adresa;
		_naziv = _adresa = nullptr;
	};

	const char* GetNaziv() const { return _naziv; };
	const char* GetAdresa() const { return _adresa; };
	vector<Gost>& GetGosti() { return _gosti; };
	vector<Rezervacija>& GetRezervacije() { return _rezervacije; };

	// Dodaje gosta. Ako gost sa istim email-om vec postoji, baca izuzetak.
	void DodajGosta(const Gost& gost) {
		for (size_t i = 0; i < _gosti.size(); i++)
		{
			if (_gosti[i] == gost) throw exception("Gost je vec bukirao hotel");
		}
		_gosti.push_back(gost);
	};

	bool validanBrojRezervacija(int minRezervacija) {
		return _rezervacije.size() >= minRezervacija;
	}

	// Kreira rezervaciju za gosta pronadjenog po email-u. Vraca true ako je uspjesno.
	bool KreirajRezervaciju(const char* emailGosta, TipSobe tipSobe,
		DatumVrijeme datumPrijave, DatumVrijeme datumOdjave,
		int brojGostiju) {
		for (size_t i = 0; i < _gosti.size(); i++)
		{
			if (strcmp(_gosti[i].GetEmail(), emailGosta) == 0) {
				Rezervacija novaRezervacija(&_gosti[i], tipSobe, datumPrijave, datumOdjave, brojGostiju, _rezervacije.size() + 1);
				_rezervacije.push_back(novaRezervacija);
				return true;
			}
		}
		return false;
	};

	// Vraca pokazivace na goste koji imaju barem minRezervacija rezervacija.
	vector<Gost*> AktivniGosti(int minRezervacija) {
		vector<Gost*> temp;
		for (size_t i = 0; i < _gosti.size(); i++)
		{
			int brojRezervacija = 0;
			for (size_t j = 0;  j < _rezervacije.size();  j++)
			{
				if (strcmp(_rezervacije[j].GetGost()->GetEmail(), _gosti[i].GetEmail()) == 0) {
					brojRezervacija++;
				}
			}
			if (brojRezervacija >= minRezervacija) {
				temp.push_back(&_gosti[i]);
			}
		}
		return temp;
	};

	// Vraca kolekciju parova (gost, ukupna potrosnja) za goste sa rezervacijama odredjenog tipa sobe.
	Kolekcija<Gost, double, 50> GostiPoTipuSobe(TipSobe tipSobe) {
		Kolekcija<Gost, double, 50> temp;
		for (size_t i = 0; i < _gosti.size(); i++)
		{
			double ukupnaPotrosnja = 0;
			for (size_t j = 0; j < _rezervacije.size(); j++)
			{
				if (_rezervacije[j].GetGost() == &_gosti[i] &&
					_rezervacije[j].GetTipSobe() == tipSobe) {
					ukupnaPotrosnja += _rezervacije[j].IzracunajUkupnuCijenu();
				}
			}
			if (ukupnaPotrosnja > 0) {
				temp.Dodaj(_gosti[i], ukupnaPotrosnja);
			}
		}
		return temp;
	};
};

bool UcitajPodatke(const string& putanja, vector<Hotel>& hoteli) {
	fstream fajl{ putanja };

	if (!fajl.is_open()) return false;

	string nazivHotela, adresa, imePrezime, email, brojTelefona, tipGostaStr, brojPosjetaStr;
	bool noviPodaci = false;

	while (getline(fajl, nazivHotela, '|') &&
		getline(fajl, adresa, '|') &&
		getline(fajl, imePrezime, '|') &&
		getline(fajl, email, '|') &&
		getline(fajl, brojTelefona, '|') &&
		getline(fajl, tipGostaStr, '|') &&
		getline(fajl, brojPosjetaStr))
	{
		// 1. Pronadji ili kreiraj hotel
		Hotel* trenutniHotel = nullptr;
		for (auto& h : hoteli) {
			if (h.GetNaziv() == nazivHotela) {
				trenutniHotel = &h;
				break;
			}
		}

		if (trenutniHotel == nullptr) {
			hoteli.push_back(Hotel(nazivHotela.c_str(), adresa.c_str()));
			trenutniHotel = &hoteli.back();
			noviPodaci = true;
		}

		// 2. Konvertuj tip gosta iz stringa u enum
		TipGosta tipGosta = REGULARNI;
		if (tipGostaStr == "VIP") tipGosta = VIP;
		else if (tipGostaStr == "POSLOVNI") tipGosta = POSLOVNI;
		else if (tipGostaStr == "PORODICA") tipGosta = PORODICA;

		// 3. Konvertuj broj posjeta
		int brojPosjeta = stoi(brojPosjetaStr);

		// 4. Provjeri da li gost vec postoji u hotelu (po email-u)
		bool gostPostoji = false;
		for (auto& g : trenutniHotel->GetGosti()) {
			if (g.GetEmail() == email) {
				gostPostoji = true;
				break;
			}
		}

		// 5. Ako gost ne postoji, kreiraj i dodaj ga
		if (!gostPostoji) {
			Gost noviGost(imePrezime.c_str(), email.c_str(), brojTelefona.c_str(),
				tipGosta, brojPosjeta, 0.0);
			trenutniHotel->DodajGosta(noviGost);
			noviPodaci = true;
		}
	}

	return noviPodaci;
}
int main() {

	//funkcija generise ID rezervacije na osnovu imena i prezimena, rednog broja, tipa sobe i trenutne godine.
//ID je u formatu HRS-IN-BB-0003/2026, gdje IN predstavlja inicijale, BB tip sobe (ST, AP, LK, SD),
//0003 redni broj popunjen nulama na cetiri mjesta, a 2026 trenutnu godinu.
//Kod imena koje sadrzi vise rijeci (srednje ime) koriste se inicijal prve i posljednje rijeci.
//Primjer: "Ana Marija Kovac" -> AK
//funkciju koristiti u konstruktoru klase Rezervacija za inicijalizaciju atributa _idRezervacije.
	if (GenerisiIDRezervacije("Amina Buric", 3, APARTMAN) == "HRS-AB-AP-0003/2026")
		cout << "ID rezervacije OK" << crt;
	if (GenerisiIDRezervacije("Amar Macic", 15, STANDARDNA) == "HRS-AM-ST-0015/2026")
		cout << "ID rezervacije OK" << crt;
	if (GenerisiIDRezervacije("Maid Ramic", 156, LUKSUZ) == "HRS-MR-LK-0156/2026")
		cout << "ID rezervacije OK" << crt;
	// Test sa srednjim imenom - uzima prvo slovo imena i prvo slovo prezimena (NE srednje ime)
	if (GenerisiIDRezervacije("Ana Marija Kovac", 42, STUDIO) == "HRS-AK-SD-0042/2026")
		cout << "ID rezervacije sa srednjim imenom OK" << crt;
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
	//VIP: 20% + 1% na svakih 5 posjeta (max 30%)
	//POSLOVNI: 15% + 0.5% na svakih 5 posjeta (max 20%)
	//PORODICA: 10% + 1% na svakih 3 posjeta (max 15%)
	//REGULARNI: 0% + 0.5% na svakih 10 posjeta (max 5%)
	cout << "Popust za Aminu: " << amina.IzracunajPopust() * 100 << "%" << crt;

	Rezervacija rez1(&amina, APARTMAN, prijava, odjava, 2, 1);

	//DodajDodatnuUslugu dodaje dodatnu uslugu rezervaciji.
	//Ako usluga sa istim nazivom vec postoji, ne dodaje je i vraca false.
	rez1.DodajDodatnuUslugu("Dorucak", 25.0);
	rez1.DodajDodatnuUslugu("Parking", 10.0);

	//ToString metoda klase Rezervacija vraca podatke u formatu:
	//HRS-AB-AP-0001/2026 | Amina Buric | APARTMAN | 10.09.2026 - 15.09.2026 | 2 gosta | KREIRANA
	cout << rez1.ToString() << crt;

	//IzracunajUkupnuCijenu racuna ukupnu cijenu rezervacije na osnovu:
	//- Cijene sobe po noci (STANDARDNA: 100KM, APARTMAN: 200KM, LUKSUZ: 350KM, STUDIO: 150KM)
	//- Broja nocenja (razlika izmedju datuma odjave i prijave)
	//- Zbira dodatnih usluga (Dorucak: 25KM, Parking: 10KM)
	//- Popusta gosta (racuna se kroz Gost::IzracunajPopust)
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