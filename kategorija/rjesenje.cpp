#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <ctime>
#include <regex>
#include <vector>
#include <fstream>
using namespace std;
char* AlocirajTekst(const char* tekst) {
	if (!tekst) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}

string crt = "\n-------------------------------------------------------------------------------- - \n";

enum Kategorija { ELEKTRONIKA, KNJIGE, MODA, PREHRANA };
const char* KategorijaNazivi[] = { "ELEKTRONIKA", "KNJIGE", "MODA", "PREHRANA" };

int getGodinuIzSistema() {
	int godina;
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	godina = (timeInfo.tm_year + 1900);
	return godina;
}

string GenerisiSifru(const char* imePrezime, int id) {
	string sifra;

	sifra += toupper(imePrezime[0]);
	const char* prezime = strchr(imePrezime, ' ');
	sifra += toupper(prezime[1]);
	sifra += ":";

	int godina = getGodinuIzSistema();
	sifra += to_string(godina);
	sifra += "-";

	if (id < 10) sifra += "00";
	else if (id < 100) sifra += "0";
	sifra += to_string(id);

	return sifra;
}

bool ValidirajSifru(string sifra) {
	return regex_match(sifra, regex("[A-Z]{2}:2026-\\d{3}"));
}

template<class T1, class T2, int max>
class KolekcijaParova {
	T1* _prvi[max] = { nullptr };
	T2* _drugi[max] = { nullptr };
	int _trenutno = 0;
public:
	KolekcijaParova() = default;
	KolekcijaParova(const KolekcijaParova& obj) {
		_trenutno = obj.GetTrenutno();
		for (size_t i = 0; i < _trenutno; i++)
		{
			_prvi[i] = new T1(*obj._prvi[i]);
			_drugi[i] = new T2(*obj._drugi[i]);
		}
	}
	KolekcijaParova& operator=(const KolekcijaParova& obj) {
		if (this != &obj)
		{
			for (int i = 0; i < _trenutno; i++) {
				delete _prvi[i]; _prvi[i] = nullptr;
				delete _drugi[i]; _drugi[i] = nullptr;
			}

			_trenutno = obj.GetTrenutno();
			for (size_t i = 0; i < _trenutno; i++)
			{
				_prvi[i] = new T1(*obj._prvi[i]);
				_drugi[i] = new T2(*obj._drugi[i]);
			}
		}
		return *this;
	}
	~KolekcijaParova() {
		for (int i = 0; i < _trenutno; i++) {
			delete _prvi[i]; _prvi[i] = nullptr;
			delete _drugi[i]; _drugi[i] = nullptr;
		}
	}
	int GetTrenutno() const { return _trenutno; }
	T1& GetPrvi(int indeks) { return *_prvi[indeks]; }
	T2& GetDrugi(int indeks) { return *_drugi[indeks]; }
	T1& operator[](int indeks) { return *_prvi[indeks]; }
	friend ostream& operator<<(ostream& COUT, KolekcijaParova& obj) {
		for (int i = 0; i < obj._trenutno; i++)
			COUT << obj.GetPrvi(i) << " " << obj.GetDrugi(i) << "\n";
		return COUT;
	}

	void Dodaj(const T1& el1, const T2& el2) {
		if (_trenutno >= max)return;

		_prvi[_trenutno] = new T1(el1);
		_drugi[_trenutno] = new T2(el2);

		_trenutno++;
	}

	KolekcijaParova<T1, T2, max> DodajNaPoziciju(int lokacija, const T1& el1, const T2& el2) {
		if (lokacija < 0 || lokacija > _trenutno) return *this;

		for (size_t i = _trenutno; i > lokacija; i--)
		{
			_prvi[i] = _prvi[i - 1];
			_drugi[i] = _drugi[i - 1];
		}

		_prvi[lokacija] = new T1(el1);
		_drugi[lokacija] = new T2(el2);

		_trenutno++;
		return *this;
	}

	KolekcijaParova<T1, T2, max>* UkloniRaspon(int start, int brojElemenata) {
		if (start < 0 || start + brojElemenata >= _trenutno) throw exception("Dati opseg nije validan");
		KolekcijaParova<T1, T2, max>* temp = new KolekcijaParova<T1, T2, max>();
		for (size_t i = start; i < start + brojElemenata; i++)
		{
			temp->Dodaj(*_prvi[i], *_drugi[i]);
		}
		for (size_t i = start; i < start + brojElemenata; i++)
		{
			delete _prvi[i]; _prvi[i] = nullptr;
			delete _drugi[i]; _drugi[i] = nullptr;
		}
		for (size_t i = start + brojElemenata; i < _trenutno; i++)
		{
			_prvi[i - brojElemenata] = _prvi[i];
			_drugi[i - brojElemenata] = _drugi[i];
		}

		_trenutno -= brojElemenata;
		return temp;
	}
};
class DatumVrijeme {
	int* _godina, * _mjesec, * _dan, * _sati, * _minute, * _sekunde;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int
		sati = 0, int minute = 0, int sekunde = 0) {
		_godina = new int(godina);
		_mjesec = new int(mjesec);
		_dan = new int(dan);
		_sati = new int(sati);
		_minute = new int(minute);
		_sekunde = new int(sekunde);
	}
	DatumVrijeme(const DatumVrijeme& obj) {
		_godina = new int(*obj._godina);
		_mjesec = new int(*obj._mjesec);
		_dan = new int(*obj._dan);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
		_sekunde = new int(*obj._sekunde);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _godina; delete _mjesec; delete _dan;
			delete _sati; delete _minute; delete _sekunde;

			_godina = new int(*obj._godina);
			_mjesec = new int(*obj._mjesec);
			_dan = new int(*obj._dan);
			_sati = new int(*obj._sati);
			_minute = new int(*obj._minute);
			_sekunde = new int(*obj._sekunde);
		}
		return *this;
	}
	~DatumVrijeme() {
		delete _godina; delete _mjesec; delete _dan;
		delete _sati; delete _minute; delete _sekunde;
	}

	int getGodina()const { return *_godina; };
	int getMjesec()const { return *_mjesec; };
	int getDan()const { return *_dan; };
	int getSati()const { return *_sati; };
	int getMinute()const { return *_minute; };
	int getSekunde()const { return *_sekunde; };

	bool operator==(const DatumVrijeme& obj) const {
		return obj.getGodina() == *_godina && obj.getMjesec() == *_mjesec && obj.getDan() == *_dan && obj.getSati() == *_sati&& obj.getMinute() == *_minute && obj.getSekunde() == *_sekunde;
	}
};

class Proizvod {
	char* _naziv;
	Kategorija _kategorija;
	int _cijena;
public:
	Proizvod(const char* naziv = "", Kategorija kategorija =
		ELEKTRONIKA, int cijena = 0)
		: _kategorija(kategorija), _cijena(cijena) {
		_naziv = AlocirajTekst(naziv);
	}
	Proizvod(const Proizvod& obj) {
		_naziv = AlocirajTekst(obj.GetNaziv());
		_kategorija = obj.GetKategorija();
		_cijena = obj.GetCijena();
	}
	Proizvod& operator=(const Proizvod& obj) {
		if (this != &obj)
		{
			delete[] _naziv;

			_naziv = AlocirajTekst(obj.GetNaziv());
			_kategorija = obj.GetKategorija();
			_cijena = obj.GetCijena();
		}
		return *this;
	}
	~Proizvod() { delete[] _naziv; }
	const char* GetNaziv() const { return _naziv; }
	Kategorija GetKategorija() const { return _kategorija; }
	int GetCijena() const { return _cijena; }
	friend ostream& operator<<(ostream& COUT, const Proizvod& p) {
		COUT << p._naziv << " " << KategorijaNazivi[(int)p._kategorija] << " " << p._cijena;
		return COUT;
	}

	bool operator==(const Proizvod& obj) const {
		return obj.GetKategorija() == _kategorija && obj.GetCijena() == _cijena && strcmp(_naziv, obj.GetNaziv()) == 0;
	}
};

class Transakcija {
protected:
	DatumVrijeme _vrijemeRealizacije;
	int _iznos;
public:
	Transakcija(DatumVrijeme vrijemeRealizacije, int iznos = 0)
		: _vrijemeRealizacije(vrijemeRealizacije), _iznos(iznos) {
	}
	Transakcija(const Transakcija& obj) {
		_vrijemeRealizacije = obj.GetVrijemeRealizacije();
		_iznos = obj.GetIznos();
	}
	Transakcija& operator=(const Transakcija& obj) {
		if (this != &obj)
		{
			_vrijemeRealizacije = obj.GetVrijemeRealizacije();
			_iznos = obj.GetIznos();
		}
		return *this;
	}
	virtual ~Transakcija() {}
	virtual string Info() const = 0;
	virtual Transakcija* GetClone() const = 0;
	const DatumVrijeme& GetVrijemeRealizacije() const {
		return
			_vrijemeRealizacije;
	}
	int GetIznos() const { return _iznos; }
};
class Kupovina : public Transakcija {
	vector<Proizvod> _kupljeniProizvodi;
public:
	Kupovina(DatumVrijeme vrijemeRealizacije) :
		Transakcija(vrijemeRealizacije, 0) {
	}
	Kupovina(const Kupovina& obj) : Transakcija(obj) {
		_kupljeniProizvodi = obj._kupljeniProizvodi;
	}
	Kupovina& operator=(const Kupovina& obj) {
		if (this!= &obj)
		{
			Transakcija::operator=(obj);
			_kupljeniProizvodi = obj._kupljeniProizvodi;
		}
		return *this;
	}
	~Kupovina() {}
	const vector<Proizvod>& GetProizvodi() const {
		return
			_kupljeniProizvodi;
	}

	string Info()const { //05.10.2025 09:30:00 KUPLJENO 1 PROIZVODA U UKUPNOM IZNOSU OD 1500KM
		stringstream ss;

		ss << setw(2) << setfill('0') << _vrijemeRealizacije.getDan() << "."
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getMjesec() << "."
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getGodina() << " "
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getSati() << ":"
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getMinute() << ":"
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getSekunde() << " ";
		ss << "KUPLJENO " << _kupljeniProizvodi.size() << " PROIZVODA U UKUPNOM IZNOSU OD " << _iznos << "KM";

		return ss.str();
	}

	void DodajProizvod(const Proizvod& proizvod) {
		_kupljeniProizvodi.push_back(proizvod);
		_iznos += proizvod.GetCijena();
	}

	Transakcija* GetClone()const { return new Kupovina(*this); };

	bool daLiJeProizvodVecKupljen(const Proizvod& proizvodZaProvjeru) const {
		for (size_t i = 0; i < _kupljeniProizvodi.size(); i++)
		{
			if (proizvodZaProvjeru == _kupljeniProizvodi[i]) return true;
		}
		return false;
	}
};

class Povrat : public Transakcija {
	vector<Proizvod> _vraceniProizvodi;
public:
	Povrat(DatumVrijeme vrijemeRealizacije) :
		Transakcija(vrijemeRealizacije, 0) {
	}
	Povrat(const Povrat& obj) :Transakcija(obj) {
		_vraceniProizvodi = obj.GetProizvodi();
	}
	Povrat& operator=(const Povrat& obj) {
		if (this != &obj)
		{
			Transakcija::operator=(obj);
			_vraceniProizvodi = obj.GetProizvodi();
		}
		return *this;
	}
	~Povrat() {};
	const vector<Proizvod>& GetProizvodi() const {
		return
			_vraceniProizvodi;
	}

	string Info()const { //05.10.2025 09:30:00 KUPLJENO 1 PROIZVODA U UKUPNOM IZNOSU OD 1500KM
		stringstream ss;

		ss << setw(2) << setfill('0') << _vrijemeRealizacije.getDan() << "."
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getMjesec() << "."
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getGodina() << " "
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getSati() << ":"
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getMinute() << ":"
			<< setw(2) << setfill('0') << _vrijemeRealizacije.getSekunde() << " ";
		ss << "VRACENO " << _vraceniProizvodi.size() << " PROIZVODA U UKUPNOM IZNOSU OD " << _iznos << "KM";

		return ss.str();
	}
	Transakcija* GetClone()const { return new Povrat(*this); };

	void DodajProizvod(const Proizvod& proizvod) {
		_vraceniProizvodi.push_back(proizvod);
		_iznos += proizvod.GetCijena();
	}
};
class Kupac {
	static int _id;
	char* _sifra;
	char* _imePrezime;
	vector<Transakcija*> _transakcije;
public:
	Kupac(const char* imePrezime) {
		_imePrezime = AlocirajTekst(imePrezime);
		_sifra = AlocirajTekst(GenerisiSifru(imePrezime, _id).c_str());
		_id++;
	}
	Kupac(const Kupac& obj) {
		_sifra = AlocirajTekst(obj.GetSifra());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		for (size_t i = 0; i < obj._transakcije.size(); i++)
		{
			_transakcije.push_back(obj._transakcije[i]->GetClone());
		}
	}
	Kupac& operator=(const Kupac& obj) {
		if (this != &obj)
		{
			delete[] _sifra;
			delete[] _imePrezime;
			for (auto* transakcija : _transakcije) delete transakcija;
			_transakcije.clear();

			_sifra = AlocirajTekst(obj.GetSifra());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			for (size_t i = 0; i < obj._transakcije.size(); i++)
			{
				_transakcije.push_back(obj._transakcije[i]->GetClone());
			}
		}
		return *this;
	}
	~Kupac() {
		delete[] _sifra;
		delete[] _imePrezime;
		for (auto* transakcija : _transakcije) delete transakcija;
		_transakcije.clear();
	}
	const char* GetSifra() const { return _sifra; }
	const char* GetImePrezime() const { return _imePrezime; }
	vector<Transakcija*>& GetTransakcije() { return _transakcije; }
	friend ostream& operator<<(ostream& COUT, Kupac& kupac) {
		COUT << crt << kupac._imePrezime << " [" << kupac._sifra <<
			"]\n";
		for (int i = 0; i < kupac._transakcije.size(); i++)
			COUT << " - " << kupac._transakcije[i]->Info() << "\n";
		COUT << crt;
		return COUT;
	}

	bool DodajTransakciju(Transakcija& transakcijaZaDodati) {
		for (size_t i = 0; i < _transakcije.size(); i++)
		{
			if(transakcijaZaDodati.GetVrijemeRealizacije() == _transakcije[i]->GetVrijemeRealizacije()) return false;
 		}
		// KUPOVINA
		if (Kupovina* novaKupovina = dynamic_cast<Kupovina*>(&transakcijaZaDodati)) {
			for (const auto& i : novaKupovina->GetProizvodi()) {
				for (const auto* t : _transakcije) {
					if (const Kupovina* staraKupovina = dynamic_cast<const Kupovina*>(t)) {
						if (staraKupovina->daLiJeProizvodVecKupljen(i) == true) return false;
					}
				}
				_transakcije.push_back(new Kupovina(*novaKupovina));
				return true;
			}
		}
		// POVRAT
		if (Povrat* povrat = dynamic_cast<Povrat*>(&transakcijaZaDodati)) {
			for (const auto& i : povrat->GetProizvodi()) {
				bool kupljen = false;
				for (const auto* t : _transakcije) {
					if (const Kupovina* kupovina = dynamic_cast<const Kupovina*>(t)) {
						if (kupovina->daLiJeProizvodVecKupljen(i) == true) kupljen = true; break;
					}
				}
				if (kupljen = false)
				{
					return false;
				}
				_transakcije.push_back(new Povrat(*povrat));
				return true;
			}
		}
		return false;
	}

	bool operator==(const Kupac& kupac)  const {
		return strcmp(kupac.GetSifra(), _sifra) == 0;
	}
};

int Kupac::_id = 1;

class Prodavnica {
	char* _naziv;
	vector<Kupac> _kupci;
public:
	Prodavnica(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	~Prodavnica() { delete[] _naziv; }
	Prodavnica(const Prodavnica& obj) {
		_naziv =
			AlocirajTekst(obj._naziv); _kupci = obj._kupci;
	}
	Prodavnica& operator=(const Prodavnica& obj) {
		if (this != &obj)
		{
			delete[] _naziv;

			_naziv =
				AlocirajTekst(obj._naziv); _kupci = obj._kupci;
		}
		return *this;
	}
	const char* GetNaziv() const { return _naziv; }
	vector<Kupac>& GetKupci() { return _kupci; }

	void DodajKupca(const Kupac& kupac) {
		for (size_t i = 0; i < _kupci.size(); i++)
		{
			if (_kupci[i] == kupac)throw exception("Kupac je vec dodat");
		}
		_kupci.push_back(kupac);
	}

	bool RegistrujTransakcijuKupcu(const char* sifraKupca, Transakcija& transakcija) {
		for (size_t i = 0; i < _kupci.size(); i++)
		{
			if (strcmp(sifraKupca, _kupci[i].GetSifra()) == 0) return _kupci[i].DodajTransakciju(transakcija);
		}
		return false;
	}

	KolekcijaParova<Kupac, int, 50> PotrosnjaPoKategoriji(Kategorija kategorija) {
		KolekcijaParova<Kupac, int, 50> temp;
		for (auto& kupac : _kupci) {
			int ukupnaPotrosnja = 0;
			for (auto* t : kupac.GetTransakcije()) {
				if (Kupovina* kupovina = dynamic_cast<Kupovina*>(t)) {
					for (auto& proizvod : kupovina->GetProizvodi()) {
						if (proizvod.GetKategorija() == kategorija) {
							ukupnaPotrosnja += proizvod.GetCijena();
						}
					}
				}
				else if (Povrat* povrat = dynamic_cast<Povrat*>(t))
				{
					for (auto& proizvod : povrat->GetProizvodi()) {
						if (proizvod.GetKategorija() == kategorija) {
							ukupnaPotrosnja -= proizvod.GetCijena();
						}
					}
				}
			}
			if (ukupnaPotrosnja != 0)
			{
				temp.Dodaj(kupac, ukupnaPotrosnja);
			}
		}
		return temp;
	}
};

bool UcitajPodatke(const string& putanja, vector<Prodavnica>& prodavnice) {
	fstream fajl{ putanja };

	if (!fajl.is_open()) return false;

	string nazivKupca;
	string nazivProdavnice;

	while (getline(fajl, nazivKupca, '|') && getline(fajl, nazivProdavnice))
	{
		Prodavnica* trenutnaProdavnica = nullptr;
		for (auto& i : prodavnice) {
			if (i.GetNaziv() == nazivProdavnice) {
				trenutnaProdavnica = &i; break;
			}
		}

		if (trenutnaProdavnica == nullptr) {
			prodavnice.push_back(Prodavnica(nazivProdavnice.c_str()));
			trenutnaProdavnica = &prodavnice.back();
		}

		bool postojiKupac = false;
		for (auto& k : trenutnaProdavnica->GetKupci()) {
			if (k.GetImePrezime() == nazivKupca) {
				postojiKupac == true; break;
			}
		}

		if (!postojiKupac) {
			trenutnaProdavnica->DodajKupca(Kupac(nazivKupca.c_str())); 
		}
	}
	return true;
}

int main() {
	
	cout << GenerisiSifru("Amina Buric", 3) << endl; 
	cout << GenerisiSifru("Amar Macic", 15) << endl; 
	cout << GenerisiSifru("Maid Ramic", 156) << endl; 

	if (ValidirajSifru("AB:2026-003"))
		cout << "SIFRA VALIDNA\n";
	if (!ValidirajSifru("Ab:2026-003") && !ValidirajSifru("AB2026/003") && !ValidirajSifru("AB-003:2026"))
		cout << "SIFRA NIJE VALIDNA\n";

	KolekcijaParova<int, string, 20> listaProizvoda;
	for (int i = 0; i < 10; i++)
		listaProizvoda.Dodaj(i, "Proizvod_" + to_string(i));
	cout << listaProizvoda << crt;

	KolekcijaParova<int, string, 20> prosirenaLista =
		listaProizvoda.DodajNaPoziciju(1, 99, "Proizvod_99");
	cout << prosirenaLista << crt;

	KolekcijaParova<int, string, 20>* uklonjeniProizvodi =
		prosirenaLista.UkloniRaspon(2, 3);
	cout << "Uklonjeni:\n" << *uklonjeniProizvodi << crt;
	cout << "Preostali:\n" << prosirenaLista << crt;

	*uklonjeniProizvodi = prosirenaLista;
	cout << "Proizvodi:\n" << *uklonjeniProizvodi << crt;
	try
	{
		listaProizvoda.UkloniRaspon(3, 10);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}

	DatumVrijeme vrijeme1(5, 10, 2025, 9, 30, 0), vrijeme2(5, 10,
		2025, 10, 15, 0), vrijeme3(5, 10, 2025, 12, 36, 0);

	Proizvod telefon("Telefon FITPhone", ELEKTRONIKA, 1500),
		knjiga("Napredno C++ programiranje", KNJIGE, 55),
		slusalice("Slusalice FSX", ELEKTRONIKA, 129), laptop("Laptop FITLx", ELEKTRONIKA, 1499);

	Kupovina kupovina1(vrijeme1), kupovinaDuplikatVremena(vrijeme1),
		kupovina2(vrijeme2), kupovinaSlusalice(vrijeme2),
		kupovinaDuplikatProizvoda(vrijeme3);

	kupovina1.DodajProizvod(telefon);
	kupovina2.DodajProizvod(knjiga);

	cout << kupovina1.Info() << endl; 

	Kupac amina("Amina Buric"), goran("Goran Skondric"), berun("Berun Agic");

	amina.DodajTransakciju(kupovina1);
	amina.DodajTransakciju(kupovina2);
	kupovinaDuplikatProizvoda.DodajProizvod(knjiga);
	if (!amina.DodajTransakciju(kupovinaDuplikatProizvoda))
		cout << "Duplikat proizvoda\n";
	kupovinaDuplikatVremena.DodajProizvod(laptop);
	if (!amina.DodajTransakciju(kupovinaDuplikatVremena))
		cout << "Duplikat vremena\n";
	Povrat povratKnjige(vrijeme2);
	povratKnjige.DodajProizvod(knjiga);
	cout << povratKnjige.Info() << endl;
	if (amina.DodajTransakciju(povratKnjige))
		cout << "Povrat uspjesno izvrsen\n";

	Prodavnica tehnika("Tehnika"), knjizara("Knjizara");
	tehnika.DodajKupca(amina);
	tehnika.DodajKupca(goran);
	knjizara.DodajKupca(berun);
	try {
		tehnika.DodajKupca(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}
	kupovinaSlusalice.DodajProizvod(slusalice);

	if (!tehnika.RegistrujTransakcijuKupcu(amina.GetSifra(), kupovinaSlusalice))
		cout << "Transakcija registrovana\n";
	vector<Prodavnica> prodavnice;
	prodavnice.push_back(tehnika);
	prodavnice.push_back(knjizara);
	
	string nazivFajla = "podaci.txt";
	if (UcitajPodatke(nazivFajla, prodavnice)) cout << "Ucitavanje uspjesno" << crt;

	for (auto& prodavnica : prodavnice)
		cout << prodavnica.GetNaziv() << " sa " << prodavnica.GetKupci().size() << " kupaca" << crt;

	KolekcijaParova<Kupac, int, 50> potrosnja = tehnika.PotrosnjaPoKategoriji(ELEKTRONIKA);
	for (int i = 0; i < potrosnja.GetTrenutno(); i++)
		cout << potrosnja.GetPrvi(i).GetImePrezime() << " [" << potrosnja.GetPrvi(i).GetSifra() << "] " << " potrosio/la " << potrosnja.GetDrugi(i) << " KM na proizvode iz kategorije ELEKTRONIKA" << crt;

	cin.get();
	return 0;
}