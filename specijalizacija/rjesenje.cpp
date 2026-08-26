#include <sstream>
#include <iostream>
#include <ctime>
#include <string>
#include <regex>
#include <iomanip>
#include <vector>
#include <fstream>
using namespace std;

enum Specializacija {
	KARDIOLOGIJA, ORTOPEDIJA, DERMATOLOGIJA, PEDIJATRIJA, OPSTA_MEDICINA
};

const char* SpecializacijaNazivi[] = {
	"KARDIOLOGIJA", "ORTOPEDIJA", "DERMATOLOGIJA", "PEDIJATRIJA", "OPSTA MEDICINA"
};

const char* crt = "\n-------------------------------------------\n";
char* AlocirajNiz(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr)return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[]sadrzaj;
	return temp;
}

char* GenerisiSifru(const char* imePrezime, int id) {
	string sifra;
	// GG
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = (timeInfo.tm_year + 1900) % 100;
	sifra += to_string(godina);
	sifra += "-";
	// IN
	sifra += toupper(imePrezime[0]);
	const char* prezime = strchr(imePrezime, ' ');
	sifra += toupper(prezime[1]);
	sifra += "-";
	// BBB
	if (id > 99) // xxx
	{
		sifra += to_string(id);
	}
	else if (id > 9) // xx
	{
		sifra += "0";
		sifra += to_string(id);
	}
	else
	{
		sifra += "00";
		sifra += to_string(id);
	}
	char* temp = new char[sifra.size() + 1];
	strcpy_s(temp, sifra.size() + 1, sifra.c_str());
	return temp;
}


bool ValidirajSifru(string sifra) {
	return regex_match(sifra, regex("\\d{2}[- ][A-Z]{2}[- ]\\d{3}"));
}

template<class T1, class T2, int max>
class KolekcijaG1 {
	T1* _elementi1[max];
	T2* _elementi2[max];
	int _trenutno;
public:
	KolekcijaG1() {
		_trenutno = 0;
		for (size_t i = 0; i < max; i++)
		{
			_elementi1[i] = nullptr;
			_elementi2[i] = nullptr;
		}
	}
	KolekcijaG1(const KolekcijaG1& obj) {
		_trenutno = obj.getTrenutno();
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = new T1(obj.getElement1(i));
			_elementi2[i] = new T2(obj.getElement2(i));
		}
	}
	KolekcijaG1& operator=(const KolekcijaG1& obj) {
		if (this != &obj)
		{
			for (size_t i = 0; i < _trenutno; i++)
			{
				delete _elementi1[i]; _elementi1[i] = nullptr;
				delete _elementi2[i]; _elementi2[i] = nullptr;
			}

			_trenutno = obj.getTrenutno();
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = new T1(*obj._elementi1[i]);
				_elementi2[i] = new T2(*obj._elementi2[i]);
			}
		}
		return *this;
	}
	~KolekcijaG1() {
		for (size_t i = 0; i < _trenutno; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
	}
	T1& getElement1(int lokacija) const { return *_elementi1[lokacija]; }
	T2& getElement2(int lokacija) const { return *_elementi2[lokacija]; }
	int getTrenutno() const { return _trenutno; }
	friend ostream& operator<< (ostream& COUT, KolekcijaG1& obj) {
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) <<
			endl;
		return COUT;
	}
	void AddElement(T1 el1, T2 el2) {
		if (_trenutno >= max) return;

		_elementi1[_trenutno] = new T1(el1);
		_elementi2[_trenutno] = new T2(el2);

		_trenutno++;
	}
	KolekcijaG1<T1, T2, max> InsertAt(int lokacija, T1 el1, T2 el2) {

		for (int i = _trenutno; i > lokacija; i--)
		{
			_elementi1[i] = new T1(*_elementi1[i - 1]);
			_elementi2[i] = new T2(*_elementi2[i - 1]);
		}

		_elementi1[lokacija] = new T1(el1);
		_elementi2[lokacija] = new T2(el2);

		_trenutno++;

		return *this;
	}
	KolekcijaG1<T1, T2, max>* RemoveRange(int start, int brojElemenata) {
		if (start < 0 || start + brojElemenata >= _trenutno) throw exception("Nema dovoljno elemenata");
		KolekcijaG1<T1, T2, max>* temp = new KolekcijaG1<T1, T2, max>();
		for (size_t i = start; i < start + brojElemenata; i++)
		{
			temp->AddElement(*_elementi1[i], *_elementi2[i]);
		}
		for (size_t i = start; i < start + brojElemenata; i++)
		{
			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}
		for (size_t i = start + brojElemenata; i < _trenutno; i++) // 0 1 null null 4 5 6
		{
			_elementi1[i - brojElemenata] = new T1(*_elementi1[i]);
			_elementi2[i - brojElemenata] = new T2(*_elementi2[i]);

			delete _elementi1[i]; _elementi1[i] = nullptr;
			delete _elementi2[i]; _elementi2[i] = nullptr;
		}

		_trenutno -= brojElemenata;
		return temp;
	}
};
class Termin {
	int* _sati; int* _minute; int* _sekunde;
public:
	Termin(int s = 0, int m = 0, int k = 0) {
		_sati = new int(s);
		_minute = new int(m);
		_sekunde = new int(k);
	}
	Termin(const Termin& obj) {
		_sati = new int(obj.getSati());
		_minute = new int(obj.getMinute());
		_sekunde = new int(obj.getSekunde());
	}
	Termin& operator=(const Termin& obj) {
		if (this != &obj)
		{
			delete _sati; delete _minute; delete _sekunde;

			_sati = new int(obj.getSati());
			_minute = new int(obj.getMinute());
			_sekunde = new int(obj.getSekunde());
		}
		return *this;
	}
	bool operator==(const Termin& obj) const {
		return *_sati == obj.getSati() && *_minute == obj.getMinute() && *_sekunde == obj.getSekunde();
	}
	~Termin() {
		delete _sati; delete _minute; delete _sekunde;
	}
	int getSati() const { return *_sati; }
	int getMinute() const { return *_minute; }
	int getSekunde() const { return *_sekunde; }
	friend ostream& operator<<(ostream& COUT, const Termin& t) {
		COUT << *t._sati << ":" << *t._minute << ":" << *t._sekunde;
		return COUT;
	}
};
class Dogadjaj {
protected:
	Termin _termin;
public:
	Dogadjaj(Termin termin) : _termin(termin) {}
	Dogadjaj(const Dogadjaj& obj) {
		_termin = obj.getTermin();
	}
	Dogadjaj& operator=(const Dogadjaj& obj) {
		if (this != &obj)
		{
			_termin = obj.getTermin();
		}
		return *this;
	}
	virtual ~Dogadjaj() {}
	virtual string Info() const = 0;
	virtual Dogadjaj* GetClone() const = 0;
	const Termin& getTermin() const { return _termin; }
};
class Predavanje : public Dogadjaj {
	char* _tema;
	Specializacija _specijalizacija;
public:
	Predavanje(const Termin& termin, const char* tema, Specializacija spec) : Dogadjaj(termin) {
		_tema = AlocirajNiz(tema);
		_specijalizacija = spec;
	}
	Predavanje(const Predavanje& obj) : Dogadjaj(obj) {
		_tema = AlocirajNiz(obj._tema);
		_specijalizacija = obj.GetSpecijalizacija();
	}
	Predavanje& operator=(const Predavanje& obj) {
		if (this != &obj)
		{

			Dogadjaj::operator=(obj);
			delete[] _tema;

			_tema = AlocirajNiz(obj._tema);
			_specijalizacija = obj.GetSpecijalizacija();
		}
		return *this;
	}
	~Predavanje() {
		delete[] _tema;
	}
	const char* GetTema() const { return _tema; }
	Specializacija GetSpecijalizacija() const {
		return _specijalizacija;
	};
	string Info() const { // info metoda vraca sve detalje o dogadjaju u string formatu npr.: 19:02:30 Oboljenja srca KARDIOLOGIJA 
		stringstream ss;
		ss << setw(2) << setfill('0') << _termin.getSati() << ":"
			<< setw(2) << setfill('0') << _termin.getMinute() << ":"
			<< setw(2) << setfill('0') << _termin.getSekunde() << " ";
		ss << _tema << " ";
		ss << SpecializacijaNazivi[_specijalizacija];
		return ss.str();
	} 
	Dogadjaj* GetClone() const {
		return new Predavanje(*this);
	}
};
	class Ucesnik {
		static int _id; //iskoristiti za praćenje rednog broja ucesnika i generisanje jedinstvene sifre
		char* _sifra; //sifra u formatu GG-IN-BBB, pojasnjena u main funkciji, generisati prilikom kreiranja objekta
		char* _imePrezime;
		vector<Dogadjaj*> _prijavljeni;
	public:
		Ucesnik(const char* imePrezime) {
			_imePrezime = AlocirajNiz(imePrezime);
			_sifra = GenerisiSifru(_imePrezime, _id++);
		}
		Ucesnik(const Ucesnik& obj) {
			_imePrezime = AlocirajNiz(obj._imePrezime);
			_sifra = AlocirajNiz(obj._sifra);
			for (size_t i = 0; i < obj._prijavljeni.size(); i++)
			{
				_prijavljeni.push_back(obj._prijavljeni[i]->GetClone());
			}
		}
		Ucesnik& operator=(const Ucesnik& obj) {
			if (this != &obj)
			{
				delete[] _imePrezime;
				delete[] _sifra;
				for (auto prijava : _prijavljeni)
					delete prijava;
				_prijavljeni.clear();

				_imePrezime = AlocirajNiz(obj._imePrezime);
				_sifra = AlocirajNiz(obj._sifra);
				for (size_t i = 0; i < _prijavljeni.size(); i++)
				{
					_prijavljeni.push_back(obj._prijavljeni[i]->GetClone());
				}
			}
			return *this;
		}
		~Ucesnik() {
			delete[] _imePrezime;
			delete[] _sifra;
			for (auto prijava : _prijavljeni)
				delete prijava;
			_prijavljeni.clear();
		}
		char* getSifra() { return _sifra; }
		const char* getSifra() const { return _sifra; }
		const char* getImePrezime() const { return _imePrezime; }
		vector<Dogadjaj*>& getDogadjaji() { return _prijavljeni; }
		bool PrijaviSe(const Predavanje& predavanje) {
			for (size_t i = 0; i < _prijavljeni.size(); i++)
			{
				if (_prijavljeni[i]->getTermin() == predavanje.getTermin()) return false;
			}
			_prijavljeni.push_back(predavanje.GetClone());
			return true;
		}
		bool operator==(const Ucesnik& obj) const {
			return strcmp(obj.getImePrezime(), _imePrezime) == 0;
		}
		bool provjeraBrojaPredavanja(int brojPredavanja) {
			return _prijavljeni.size() >= brojPredavanja;
		}
	};

	int Ucesnik::_id = 1;


	class Tim {
		char* _naziv;
		vector<Ucesnik> _clanovi;
	public:
		Tim(const char* naziv) {
			_naziv = AlocirajNiz(naziv);
		}
		Tim(const Tim& obj) {
			_naziv = AlocirajNiz(obj._naziv);
			_clanovi = obj._clanovi;
		}
		Tim& operator=(const Tim& obj) {
			if (this != &obj)
			{
				delete[] _naziv;

				_naziv = AlocirajNiz(obj._naziv);
				_clanovi = obj._clanovi;
			}
			return *this;
		}
		~Tim() {
			delete[] _naziv;
		}

		void DodajUcesnika(const Ucesnik& ucesnik) {
			for (size_t i = 0; i < _clanovi.size(); i++)
			{
				if (ucesnik == _clanovi[i]) throw exception("Ucesnik je vec clan tima");
			}

			_clanovi.push_back(ucesnik);
		}
		friend ostream& operator<<(ostream& COUT, const Tim& obj) {
			COUT << obj._naziv << ": " << endl;
			for (size_t i = 0; i < obj._clanovi.size(); i++)
			{
				COUT << obj._clanovi[i].getImePrezime() << endl;
			}
			return COUT;
		}
		char* getNaziv() const {
			return _naziv;
		}
		vector<Ucesnik>& getUcesnici() { return _clanovi; };
		bool provjeraClana(const char* sifraUcesnika) {
			for (size_t i = 0; i < _clanovi.size(); i++)
			{
				if (strcmp(sifraUcesnika, _clanovi[i].getSifra()) == 0)return true;
			}
			return false;
		}
	};

	class Konferencija {
		char* _naziv;
		KolekcijaG1<Tim*, Tim*, 20> _timovi;
	public:
		Konferencija(const char* naziv)
		{
			_naziv = AlocirajNiz(naziv);
		}
		Konferencija(const Konferencija& obj) {
			_naziv = AlocirajNiz(obj._naziv);
			_timovi = obj._timovi;
		}
		Konferencija& operator=(const Konferencija& obj) {
			if (this != &obj)
			{
				delete[] _naziv; _naziv = nullptr;

				_naziv = AlocirajNiz(obj._naziv);
				_timovi = obj._timovi;
			}
			return *this;
		}
		~Konferencija() {
			delete[] _naziv; _naziv = nullptr;
		}
		char* getNaziv() const { return _naziv; }
		KolekcijaG1<Tim*, Tim*, 20>& getTimovi() { return _timovi; }
		void DodajTimove(const Tim& tim1, const Tim& tim2) {
			_timovi.AddElement(new Tim(tim1), new Tim(tim2));
		}
		friend ostream& operator<<(ostream& COUT, const Konferencija& obj) {
			COUT << "Konferencija: " << obj.getNaziv() << endl;
			for (size_t i = 0; i < obj._timovi.getTrenutno(); i++)
			{
				COUT << *obj._timovi.getElement1(i) << "\n" << *obj._timovi.getElement2(i) << endl;
			}
			return COUT;
		}
		bool PostojeciTim(const char* imeTima) {
			for (size_t i = 0; i < _timovi.getTrenutno(); i++)
			{
				if (strcmp(imeTima, _timovi.getElement1(i)->getNaziv()) == 0 || strcmp(imeTima, _timovi.getElement2(i)->getNaziv()) == 0) return true;
			}
			return false;
		}

		bool PrijaviDogadjaj(const char* nazivTima, const char* sifraUcesnika, const Predavanje& predavanje) {
			if (!PostojeciTim(nazivTima))return false;
			for (size_t i = 0; i < _timovi.getTrenutno(); i++)
			{
				if (strcmp(nazivTima, _timovi.getElement1(i)->getNaziv()) == 0 || strcmp(nazivTima, _timovi.getElement2(i)->getNaziv()) == 0) {
					if (strcmp(nazivTima, _timovi.getElement1(i)->getNaziv()) == 0)
					{
						if (!_timovi.getElement1(i)->provjeraClana(sifraUcesnika)) return false;
						for (size_t j = 0; j < _timovi.getElement1(i)->getUcesnici().size(); j++)
						{
							if (strcmp(sifraUcesnika, _timovi.getElement1(i)->getUcesnici()[j].getSifra()) == 0)
							{
								return _timovi.getElement1(i)->getUcesnici()[j].PrijaviSe(predavanje);
							}
						}
					}
					else
					{
						if (!_timovi.getElement2(i)->provjeraClana(sifraUcesnika)) return false;
						for (size_t j = 0; j < _timovi.getElement2(i)->getUcesnici().size(); j++)
						{
							if (strcmp(sifraUcesnika, _timovi.getElement2(i)->getUcesnici()[j].getSifra()) == 0)
							{
								return _timovi.getElement2(i)->getUcesnici()[j].PrijaviSe(predavanje);
							}
						}
					}
				}
			}
			return false;
		}
		vector<Ucesnik*> operator()(int brojPredavanja) {
			vector<Ucesnik*> temp;
			for (size_t i = 0; i < _timovi.getTrenutno(); i++)
			{
				for (size_t j = 0; j < _timovi.getElement1(i)->getUcesnici().size(); j++)
				{
					if (_timovi.getElement1(i)->getUcesnici()[j].provjeraBrojaPredavanja(brojPredavanja))
					{
						temp.push_back(new Ucesnik(_timovi.getElement1(i)->getUcesnici()[j]));
					}
				}
				for (size_t j = 0; j < _timovi.getElement2(i)->getUcesnici().size(); j++)
				{
					if (_timovi.getElement2(i)->getUcesnici()[j].provjeraBrojaPredavanja(brojPredavanja))
					{
						temp.push_back(new Ucesnik(_timovi.getElement2(i)->getUcesnici()[j]));
					}
				}
			}
			return temp;
		}
	};

	bool UcitajUcesnike(const string& putanja, vector<Tim>& timovi) {
		fstream fajl{ putanja };

		if (!fajl.is_open()) return false;

		string nazivUcesnika;
		string nazivTima;

		while (getline(fajl, nazivUcesnika, '|') && getline(fajl, nazivTima))
		{
			Tim* trenutniTim = nullptr;
			for (auto& t : timovi) {
				if (t.getNaziv() == nazivTima) {
					trenutniTim = &t; break;
				}
			}
			if (trenutniTim == nullptr) {
				timovi.push_back(Tim(nazivTima.c_str()));
				trenutniTim = &timovi.back();
			}

			bool postojiUcesnik = false;
			for (auto& u : trenutniTim->getUcesnici()) {
				if (u.getImePrezime() == nazivUcesnika) {
					postojiUcesnik = true; break;
				}
			}
			if (!postojiUcesnik)
			{
				trenutniTim->DodajUcesnika(Ucesnik(nazivUcesnika.c_str()));
			}
		}
		return true;
	}

	int main() {

		cout << GenerisiSifru("Denis Music", 3) << endl;
		cout << GenerisiSifru("Jasmin Azemovic", 14) << endl;
		cout << GenerisiSifru("Goran skondric", 156) << endl;
		cout << GenerisiSifru("emina junuz", 798) << endl;

		cout << "-------------------------------------------" << endl;

		if (ValidirajSifru("25-DM-003"))
			cout << "SIFRA VALIDNA" << endl;
		if (ValidirajSifru("25-JA-014") && ValidirajSifru("25-JA 014"))
			cout << "SIFRA VALIDNA" << endl;
		if (!ValidirajSifru("25-GS-15") || !ValidirajSifru("25-Gs-135") || !ValidirajSifru("25-GS-153G"))
			cout << "SIFRA NIJE VALIDNA" << endl;

		cout << "-------------------------------------------" << endl;

		int kolekcijaTestSize = 9;
		KolekcijaG1<int, string, 10> kolekcija1;
		for (int i = 0; i < kolekcijaTestSize; i++)
			kolekcija1.AddElement(i, "Vrijednost -> " + to_string(i));
		cout << kolekcija1 << crt;

		KolekcijaG1<int, string, 10> kolekcija2 = kolekcija1.InsertAt(1, 10, "Vrijednost -> 10");
		cout << kolekcija2 << crt;


		KolekcijaG1<int, string, 10>* kolekcija3 =
			kolekcija1.RemoveRange(1, 3);
		cout << "Uklonjeni:\n" << *kolekcija3;
		cout << "Preostali:\n" << kolekcija1;
		try {
			kolekcija3->RemoveRange(2, 3);
		}
		catch (exception& e) {
			cout << "Exception: " << e.what() << crt;
		}
		delete kolekcija3;
		kolekcija1 = kolekcija2;
		cout << kolekcija1 << crt;


		Termin termin1(19, 02, 30), termin2(10, 30, 40), termin3(14, 15, 20), termin4(16, 45, 20);

		Predavanje oboljenja_srca(termin1, "Oboljenja srca", KARDIOLOGIJA);
		Predavanje uv_zracenja(termin2, "Uloga UV zracenja u koznim oboljenjima", DERMATOLOGIJA);
		Predavanje anemije(termin3, "Anemije u svakodnevnoj praksi", OPSTA_MEDICINA);

		Ucesnik emina("Emina Junuz"), goran("Goran Skondric"), azra("Azra Maric"), tajib("Tajib Hero");

		emina.PrijaviSe(anemije);
		emina.PrijaviSe(oboljenja_srca);
		goran.PrijaviSe(oboljenja_srca);
		goran.PrijaviSe(uv_zracenja);
		tajib.PrijaviSe(uv_zracenja);

		if (!emina.PrijaviSe(oboljenja_srca)) {
			cout << "Ucesnik " << emina.getImePrezime() << " vec prijavljen na predavanje " << oboljenja_srca.Info() << crt;
		}

		Tim timAlpha("Tim Alpha"), timBeta("Tim Beta");

		timAlpha.DodajUcesnika(emina); timAlpha.DodajUcesnika(goran);
		timBeta.DodajUcesnika(azra); timBeta.DodajUcesnika(tajib);
		try {
			timAlpha.DodajUcesnika(emina);//emina je vec clanica tima Alpha
		}
		catch (exception& e) {
			cout << "Exception: " << e.what() << "\n";
		}

		Konferencija savremena_medicina("Umjetna inteligencija u dijagnostici i lijecenju – novo lice medicine");
		savremena_medicina.DodajTimove(timAlpha, timBeta);

		cout << savremena_medicina;

		if (savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), emina.getSifra(), uv_zracenja))
			cout << "Prijava uspjesna" << crt; // DONE
		if (!savremena_medicina.PrijaviDogadjaj("Tim Gamma", emina.getSifra(), anemije))
			cout << "Pokusaj prijave dogadjaja za nepostojeci tim" << crt; // DONE
		if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), emina.getSifra(), uv_zracenja))
			cout << "Pokusaj dupliranja prijave predavanja" << crt;
		if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), azra.getSifra(), uv_zracenja))
			cout << "Ucesnik nije clan proslijedjenog tima" << crt;
		if (!savremena_medicina.PrijaviDogadjaj(timAlpha.getNaziv(), "24-GX-002", anemije))
			cout << "Prijava sa nepostojecom sifrom nije uspjela." << crt;


		vector<Ucesnik*> vrijedniUcesnici = savremena_medicina(2);
		for (auto ucesnik : vrijedniUcesnici)
			cout << ucesnik->getImePrezime() << "\n";

		/*Funkcija UcitajUcesnike ima zadatak ucitati podatke o ucesnicima i
	   njihovim timovima iz fajla cije ime se proslijedjuje kao parametar
		(fajl mozete pronaci zajedno sa ispitnim zadatkom). Svaka linija u
	   fajlu treba biti u formatu "ime i prezime|naziv tima". Funkcija za
		svaki red u fajlu:
		- unutar vector-a, po nazivu, pronadje ranije dodati ili
	   kreira novi tim,
		- kreira novog ucesnika ukoliko vec nije dio tog tima,
		- dodaje ucesnika u odgovarajuci tim (onemoguciti dupliciranje
	   korisnika u istom timu).
		Na kraju, svi timovi sa svojim clanovima se nalaze u
	   proslijedjenom vektoru timovi.
		Funkcija vraca true ako je ucitavanje podataka bilo ouspjesno, a
	   false ako se desilo nesto neocekivano.
		Primjer sadrzaja fajla:
		Goran Skondric|Tim Alpha
		Emina Junuz|Tim Alpha
		Azra Maric|Tim Beta
		Tajib Hero|Tim Beta
		*/
		vector<Tim> timoviIzFajla;
		if (UcitajUcesnike("ucesnici.txt", timoviIzFajla))
			cout << "Ucitavanje podataka USPJESNO.\n";
		else
			cout << "Ucitavanje podataka NEUSPJESNO.\n";
		for (auto& tim : timoviIzFajla)
			cout << tim << crt;
		cin.get();
		return 0;
	}