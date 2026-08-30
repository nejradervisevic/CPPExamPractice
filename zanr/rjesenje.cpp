#include <iostream>
#include <ctime>
#include <string>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <regex>
#include <vector>
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
	if (!tekst) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}
int getGodinu() {
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = timeInfo.tm_year + 1900;
	return godina;
}
//funkcija generise clanski broj na osnovu imena i prezimena, rednog broja i trenutne godine.
	//clanski broj je u formatu GGGG/IN-BBB, gdje GGGG predstavlja trenutnu godinu, IN inicijale,
	//a BBB redni broj clana popunjen nulama na tri mjesta.
	//funkciju koristiti u konstruktoru klase ClanBiblioteke za inicijalizaciju atributa _clanskiBroj.
string GenerisiClanskiBroj(const char* imePrezime, int redniBroj) {
	string clBr;

	int godina = getGodinu();
	clBr += to_string(godina);
	clBr += "/";

	clBr += toupper(imePrezime[0]);
	const char* prezime = strchr(imePrezime, ' ');
	if (prezime != nullptr && strlen(prezime) > 1) clBr += toupper(prezime[1]);
	clBr += "-";

	if (redniBroj < 10) clBr += "00";
	else if (redniBroj < 100) clBr += "0";
	clBr += to_string(redniBroj);

	return clBr;
};
bool ValidirajClanskiBroj(const string& clanskiBroj) {
	return regex_match(clanskiBroj, regex("2026/[A-Z]{2}-\\d{3}"));
};

enum Zanr {
	ROMAN, STRUCNA_LITERATURA, BIOGRAFIJA, POEZIJA, DJECIJA_KNJIGA
};

const char* ZanrNazivi[] = { "ROMAN", "STRUCNA LITERATURA", "BIOGRAFIJA", "POEZIJA", "DJECIJA KNJIGA" };

template<class T1, class T2, int max>
class Kolekcija {
	T1* _prvi;
	T2* _drugi;
	int _trenutno;
public:
	Kolekcija() : _prvi(nullptr), _drugi(nullptr), _trenutno(0) {}
	int GetTrenutno() const { return _trenutno; }
	T1& GetPrvi(int indeks) { return _prvi[indeks]; }
	T2& GetDrugi(int indeks) { return _drugi[indeks]; }
	const T1& GetPrvi(int indeks) const { return _prvi[indeks]; }
	const T2& GetDrugi(int indeks) const { return _drugi[indeks]; }
	T1& operator[](int indeks) { return _prvi[indeks]; }
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetPrvi(i) << " " << obj.GetDrugi(i) << endl;
		return COUT;
	}
	Kolekcija(const Kolekcija& obj) {
		_trenutno = obj.GetTrenutno();

		_prvi = new T1[_trenutno];
		_drugi = new T2[_trenutno];

		for (size_t i = 0; i < _trenutno; i++)
		{
			_prvi[i] = obj.GetPrvi(i);
			_drugi[i] = obj.GetDrugi(i);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete[] _prvi; _prvi = nullptr;
			delete[] _drugi; _drugi = nullptr;

			_trenutno = obj.GetTrenutno();

			_prvi = new T1[_trenutno];
			_drugi = new T2[_trenutno];

			for (size_t i = 0; i < _trenutno; i++)
			{
				_prvi[i] = obj.GetPrvi(i);
				_drugi[i] = obj.GetDrugi(i);
			}
		}
		return *this;
	}
	~Kolekcija() {
		delete[] _prvi; _prvi = nullptr;
		delete[] _drugi; _drugi = nullptr;
	}

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
	}

	Kolekcija DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2) {
		if (_trenutno >= max) throw exception("Kolekcija puna");
		if (pozicija < 0 || pozicija > _trenutno) throw exception("Pozicija nije validna");

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
	}

	Kolekcija* UkloniRaspon(int start, int brojElemenata) {
		if (_trenutno >= max || start < 0 || start + brojElemenata >= _trenutno) throw exception("Dati opseg nije validan");

		Kolekcija* temp = new Kolekcija();
		for (size_t i = start; i < start + brojElemenata; i++)
		{
			temp->Dodaj(_prvi[i], _drugi[i]);
		}

		for (size_t i = start  +brojElemenata; i < _trenutno; i++)
		{
			_prvi[i - brojElemenata] = _prvi[i];
			_drugi[i - brojElemenata] = _drugi[i];
		}

		_trenutno -= brojElemenata;
		return temp;
	}
};

class Datum {
	int* _godina, * _mjesec, * _dan;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_godina = new int(godina);
		_mjesec = new int(mjesec);
		_dan = new int(dan);
	}
	Datum(const Datum& obj) {
		_godina = new int(*obj._godina);
		_mjesec = new int(*obj._mjesec);
		_dan = new int(*obj._dan);
	}
	Datum& operator=(const Datum& obj) {
		if (this != &obj)
		{
			delete _godina; delete _mjesec; delete _dan;

			_godina = new int(*obj._godina);
			_mjesec = new int(*obj._mjesec);
			_dan = new int(*obj._dan);
		}
		return *this;
	}
	~Datum() {
		delete _godina; delete _mjesec; delete _dan;
	}

	const int getDan() const { return *_dan; };
	const int getMjesec() const { return *_mjesec; };
	const int getGodina() const { return *_godina; };
};

class Posudba {
	char* _naslov;
	Zanr _zanr;
	Datum _datumPosudbe;
	int _brojDana;
public:
	Posudba(const char* naslov, Zanr zanr, Datum datumPosudbe, int
		brojDana)
		: _zanr(zanr), _datumPosudbe(datumPosudbe),
		_brojDana(brojDana) {
		_naslov = AlocirajTekst(naslov);
	}
	Posudba(const Posudba& obj) {
		_naslov = AlocirajTekst(obj.GetNaslov());
		_zanr = obj.GetZanr();
		_datumPosudbe = obj._datumPosudbe;
		_brojDana = obj.GetBrojDana();
	}
	Posudba& operator=(const Posudba& obj) {
		if (this != &obj)
		{
			delete[] _naslov; _naslov = nullptr;

			_naslov = AlocirajTekst(obj.GetNaslov());
			_zanr = obj.GetZanr();
			_datumPosudbe = obj._datumPosudbe;
			_brojDana = obj.GetBrojDana();
		}
		return *this;
	}
	~Posudba() { delete[] _naslov; _naslov = nullptr; }
	const char* GetNaslov() const { return _naslov; }
	Zanr GetZanr() const { return _zanr; }
	Datum& GetDatumPosudbe() { return _datumPosudbe; }
	int GetBrojDana() const { return _brojDana; }
	string ToString() {
		stringstream ss;

		ss << setw(2) << setfill('0') << GetDatumPosudbe().getDan() << "."
			<< setw(2) << setfill('0') << GetDatumPosudbe().getMjesec() << "."
			<< setw(2) << setfill('0') << GetDatumPosudbe().getGodina() << " ";

		ss << GetNaslov() << " " << ZanrNazivi[GetZanr()] << " " << GetBrojDana() << " dana";

		return ss.str();
	}
	bool operator==(const Posudba& obj) const {
		return strcmp(GetNaslov(), obj.GetNaslov()) == 0;
	}
};

class ClanBiblioteke {
	static int _id;
	char* _clanskiBroj;
	char* _imePrezime;
	vector<Posudba> _posudbe;
public:
	ClanBiblioteke(const char* imePrezime = "") {
		_imePrezime = AlocirajTekst(imePrezime);
		_clanskiBroj = AlocirajTekst(GenerisiClanskiBroj(imePrezime,
			_id).c_str());
		_id++;
	}
	ClanBiblioteke(const ClanBiblioteke& obj) {
		_clanskiBroj = AlocirajTekst(obj.GetClanskiBroj());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_posudbe = obj._posudbe;
	}
	ClanBiblioteke& operator=(const ClanBiblioteke& obj) {
		if (this != &obj)
		{
			delete[] _clanskiBroj; _clanskiBroj = nullptr;
			delete[] _imePrezime; _imePrezime = nullptr;

			_clanskiBroj = AlocirajTekst(obj.GetClanskiBroj());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_posudbe = obj._posudbe;
		}
		return *this;
	}
	~ClanBiblioteke() {
		delete[] _clanskiBroj; _clanskiBroj = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
	}
	const char* GetClanskiBroj() const { return _clanskiBroj; }
	const char* GetImePrezime() const { return _imePrezime; }
	vector<Posudba>& GetPosudbe() { return _posudbe; }
	friend ostream& operator<<(ostream& COUT, ClanBiblioteke& obj) {
		COUT << obj._imePrezime << " [" << obj._clanskiBroj << "]" <<
			endl;
		for (auto& posudba : obj._posudbe)
			//ToString metoda vraca podatke o posudbi u formatu prikazanom u nastavku.
			//voditi racuna o prikazu jednocifrenih vrijednosti  datuma(npr. 5 -> 05).
			//05.10.2026 Tvrdjava ROMAN 14 dana
			COUT << " - " << posudba.ToString() << endl;
		return COUT;
	}
	//DodajPosudbu dodaje posudbu ako clan nema ranije zaduzenu knjigu istog naslova,
	//ako trenutno ima manje od tri posudbe i ako je broj dana posudbe u rasponu od 1 do 30.
	//metoda vraca true ako je posudba dodana, u suprotnom vraca false.
	bool DodajPosudbu(const Posudba& novaPosudba) {
		for (size_t i = 0; i < _posudbe.size(); i++)
		{
			if (_posudbe[i] == novaPosudba) return false;
			if (_posudbe[i].GetBrojDana() < 1 || _posudbe[i].GetBrojDana() > 30) return false;
		}
		if (_posudbe.size() >= 3) return false;
		
		_posudbe.push_back(novaPosudba);
		return true;
	}
	//RazduziKnjigu uklanja posudbu na osnovu naslova knjige i vraca true ako je pronadjena
	//i uklonjena. ukoliko knjiga nije pronadjena metoda vraca false.
	bool RazduziKnjigu(const char* naslovKnjige) {
		for (size_t i = 0; i < _posudbe.size(); i++)
		{
			if (strcmp(naslovKnjige, _posudbe[i].GetNaslov()) == 0) {
				_posudbe.erase(_posudbe.begin() + i);
				return true;
			}
		}
		return false;
	}
	bool operator==(const ClanBiblioteke& obj) const {
		return strcmp(obj.GetClanskiBroj(), GetClanskiBroj()) == 0;
	}
};
int ClanBiblioteke::_id = 1;

class Biblioteka {
	char* _naziv;
	vector<ClanBiblioteke> _clanovi;
public:
	Biblioteka(const char* naziv) { _naziv = AlocirajTekst(naziv); }
	~Biblioteka() { delete[] _naziv; _naziv = nullptr; }
	Biblioteka(const Biblioteka& obj) {
		_naziv = AlocirajTekst(obj._naziv);
		_clanovi = obj._clanovi;
	}
	Biblioteka& operator=(const Biblioteka& obj) {
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj._naziv);
			_clanovi = obj._clanovi;
		}
		return *this;
	}
	const char* GetNaziv() const { return _naziv; }
	vector<ClanBiblioteke>& GetClanovi() { return _clanovi; }
	void DodajClana(const ClanBiblioteke& noviClan) {
		for (size_t i = 0; i < _clanovi.size(); i++)
		{
			if (_clanovi[i] == noviClan) throw exception("Clan je vec dodan");
		}
		_clanovi.push_back(noviClan);
	}
	bool EvidentirajPosudbu(const char* clanskiBroj, const Posudba& novaPosudba) {
		for (size_t i = 0; i < _clanovi.size(); i++)
		{
			if (strcmp(_clanovi[i].GetClanskiBroj(), clanskiBroj) == 0) {
				_clanovi[i].DodajPosudbu(novaPosudba);
				return true;
			}
		}
		return false;
	}
	//AktivniClanovi vraca pokazivace na clanove koji imaju najmanje onoliko posudbi
	//koliko je definisano vrijednoscu proslijedjenog parametra.
	vector<ClanBiblioteke*> AktivniClanovi(int minPosudbi) {
		vector<ClanBiblioteke*> temp;
		for (size_t i = 0; i < _clanovi.size(); i++)
		{
			if (_clanovi[i].GetPosudbe().size() >= minPosudbi) temp.push_back(&_clanovi[i]);
		}
		return temp;
	}
	//PosudbePoZanru vraca kolekciju parova (clan, broj posudbi) za sve clanove koji imaju
	//najmanje jednu aktivnu posudbu knjige proslijedjenog zanra.
	Kolekcija<ClanBiblioteke, int, 50> PosudbePoZanru(Zanr zanr) {
		Kolekcija<ClanBiblioteke, int, 50> temp;
		for (size_t i = 0; i < _clanovi.size(); i++)
		{
			int brojPosudbi = 0;
			for (size_t j = 0; j < _clanovi[i].GetPosudbe().size(); j++)
			{
				if (_clanovi[i].GetPosudbe()[j].GetZanr() == zanr) brojPosudbi++;
			}
			if (brojPosudbi > 0)
			{
				temp.Dodaj(_clanovi[i], brojPosudbi);
			}
		}
		return temp;
	}
};

bool UcitajPodatke(const string& putanja, vector<Biblioteka>& biblioteke) {
	fstream fajl{ putanja };
	
	if (!fajl.is_open()) return false;

	string nazivClana;
	string nazivBiblioteke;

	bool ucitan = false;

	while (getline(fajl, nazivBiblioteke, '|') && getline(fajl, nazivClana))
	{
		Biblioteka* trenutnaBiblioteka = nullptr;
		for (auto& b : biblioteke) {
			if (b.GetNaziv() == nazivBiblioteke) {
				trenutnaBiblioteka = &b;
				break;
			}
		}
		if (trenutnaBiblioteka == nullptr) {
			biblioteke.push_back(Biblioteka(nazivBiblioteke.c_str()));
			trenutnaBiblioteka = &biblioteke.back();
		}

		bool postoji = false;
		for (auto& c : trenutnaBiblioteka->GetClanovi()) {
			if (c.GetImePrezime() == nazivClana) {
				postoji = true;
				break;
			}
		}
		if (!postoji)
		{
			trenutnaBiblioteka->DodajClana(ClanBiblioteke(nazivClana.c_str()));
			ucitan = true;
		}
	}
	return ucitan;
}

int main() {
	//funkcija generise clanski broj na osnovu imena i prezimena, rednog broja i trenutne godine.
	//clanski broj je u formatu GGGG/IN-BBB, gdje GGGG predstavlja trenutnu godinu, IN inicijale,
	//a BBB redni broj clana popunjen nulama na tri mjesta.
	//funkciju koristiti u konstruktoru klase ClanBiblioteke za inicijalizaciju atributa _clanskiBroj.
	if (GenerisiClanskiBroj("Amina Buric", 3) == "2026/AB-003")
		cout << "Clanski broj OK" << crt;
	if (GenerisiClanskiBroj("Amar Macic", 15) == "2026/AM-015")
		cout << "Clanski broj OK" << crt;
	if (GenerisiClanskiBroj("Maid Ramic", 156) == "2026/MR-156")
		cout << "Clanski broj OK" << crt;
	//ValidirajClanskiBroj koristeci regex provjerava format definisan u prethodnom dijelu zadatka.
	if (ValidirajClanskiBroj("2026/AB-003"))
		cout << "CLANSKI BROJ VALIDAN" << crt;
	if (!ValidirajClanskiBroj("2026/Ab-003") &&
		!ValidirajClanskiBroj("26/AB-003") &&
		!ValidirajClanskiBroj("2026-AB/003"))
		cout << "CLANSKI BROJ NIJE VALIDAN" << crt;
	Kolekcija<int, string, 20> inventar;
	for (int i = 0; i < 8; i++)
		inventar.Dodaj(i, "Knjiga_" + to_string(i));
	cout << inventar << crt;
	//DodajNaPoziciju dodaje novi par na lokaciju definisanu prvim parametrom metoda vraca trenutno stanje kolekcije.
	//u slucaju popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak(za potrebe testiranja mozete dodati try catch blok).
	Kolekcija<int, string, 20> prosireniInventar = inventar.DodajNaPoziciju(2, 99, "Posebno izdanje");
	cout << prosireniInventar << crt;
	//UkloniRaspon uklanja broj elemenata definisan drugim parametrom, pocevsi od lokacije
	//definisane prvim parametrom (ukljucujuci tu lokaciju). metoda vraca pokazivac na novu kolekciju sa uklonjenim
	//elementima, a pozivalac je odgovoran za njenu dealokaciju.
	Kolekcija<int, string, 20>* uklonjeneKnjige = prosireniInventar.UkloniRaspon(3, 2);
	cout << "Uklonjeni elementi:" << crt << *uklonjeneKnjige;
	cout << "Preostali elementi:" << crt << prosireniInventar;
	delete uklonjeneKnjige;
	try {
		//za neispravan raspon potrebno je baciti izuzetak
		inventar.UkloniRaspon(6, 5);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}
	Datum datum1(5, 10, 2026), datum2(7, 10, 2026), datum3(10, 10,
		2026);
	Posudba tvrdjava("Tvrdjava", ROMAN, datum1, 14);
	Posudba cpp("Programiranje u C++", STRUCNA_LITERATURA, datum1,
		30);
	Posudba tesla("Nikola Tesla", BIOGRAFIJA, datum2, 20);
	Posudba pjesme("Izabrane pjesme", POEZIJA, datum3, 10);
	//ToString metoda vraca podatke o posudbi u formatu prikazanom u nastavku.
	//voditi racuna o prikazu jednocifrenih vrijednosti datuma (npr. 5 -> 05).
	cout << tvrdjava.ToString() << crt;
	//05.10.2026 Tvrdjava ROMAN 14 dana
	ClanBiblioteke amina("Amina Buric"), goran("Goran Skondric"), berun("Berun Agic");
	//DodajPosudbu dodaje posudbu ako clan nema ranije zaduzenu knjigu istog naslova,
	//ako trenutno ima manje od tri posudbe i ako je broj dana posudbe u rasponu od 1 do 30.
	//metoda vraca true ako je posudba dodana, u suprotnom vraca false.
	if (amina.DodajPosudbu(tvrdjava))
		cout << "Posudba dodana" << crt;
	if (!amina.DodajPosudbu(tvrdjava))
		cout << "Posudba nije dodana - knjiga je vec zaduzena" << crt;
	amina.DodajPosudbu(cpp);
	amina.DodajPosudbu(tesla);
	if (!amina.DodajPosudbu(pjesme))
		cout << "Posudba nije dodana - dostignut maksimalan broj posudbi" << crt;
	//RazduziKnjigu uklanja posudbu na osnovu naslova knjige i vraca true ako je pronadjena
	//i uklonjena. ukoliko knjiga nije pronadjena metoda vraca false.
	if (amina.RazduziKnjigu("Tvrdjava"))
		cout << "Knjiga razduzena" << crt;
	if (!amina.RazduziKnjigu("Nepostojeca knjiga"))
		cout << "Knjiga nije pronadjena" << crt;
	Biblioteka gradska("Gradska biblioteka"),
		univerzitetska("Univerzitetska biblioteka");
	gradska.DodajClana(amina);
	gradska.DodajClana(goran);
	univerzitetska.DodajClana(berun);
	try {
		//DodajClana onemogucava dodavanje clana sa istim clanskim brojem i baca izuzetak
		gradska.DodajClana(amina);
	}
	catch (exception& e) {
		cout << "Exception: " << e.what() << crt;
	}
	//EvidentirajPosudbu pronalazi clana na osnovu clanskog broja i dodaje mu posudbu.
	//i dalje vaze pravila definisana u metodi DodajPosudbu. metoda vraca true ili false.
	if (gradska.EvidentirajPosudbu(goran.GetClanskiBroj(), pjesme))
		cout << "Posudba evidentirana" << crt;
	//AktivniClanovi vraca pokazivace na clanove koji imaju najmanje onoliko posudbi
	//koliko je definisano vrijednoscu proslijedjenog parametra.
	vector<ClanBiblioteke*> aktivni = gradska.AktivniClanovi(1);
	for (auto clan : aktivni)
		cout << clan->GetImePrezime() << " ima " << clan->GetPosudbe().size() << " aktivnih posudbi" << crt;
	//PosudbePoZanru vraca kolekciju parova (clan, broj posudbi) za sve clanove koji imaju
	//najmanje jednu aktivnu posudbu knjige proslijedjenog zanra.
	Kolekcija<ClanBiblioteke, int, 50> strucnePosudbe = gradska.PosudbePoZanru(STRUCNA_LITERATURA);
	for (int i = 0; i < strucnePosudbe.GetTrenutno(); i++)
		cout << strucnePosudbe.GetPrvi(i).GetImePrezime() << " -> " << strucnePosudbe.GetDrugi(i) << " posudbi" << crt;
	vector<Biblioteka> biblioteke;
	biblioteke.push_back(gradska);
	biblioteke.push_back(univerzitetska);
	/*
	Funkcija UcitajPodatke ucitava podatke o bibliotekama i njihovim
   clanovima iz datoteke
	cije ime se prosljedjuje kao prvi parametar. Svaka linija je
   zapisana u formatu:
	naziv biblioteke|ime i prezime clana
	Za svaki ispravan red potrebno je:
	- pronaci postojecu ili kreirati novu biblioteku,
	- kreirati i dodati clana u odgovarajucu biblioteku,
	- onemoguciti dupliranje biblioteka i clanova unutar iste
   biblioteke.
	Funkcija vraca true ako je ucitan najmanje jedan novi podatak, a
   false ako datoteka ne
	postoji ili nije ucitan nijedan novi podatak.
	Primjer sadrzaja datoteke:
	Gradska biblioteka|Emina Junuz
	Gradska biblioteka|Jasmin Azemovic
	Univerzitetska biblioteka|Zanin Vejzovic
	*/
	if (UcitajPodatke("clanovi.txt", biblioteke))
		cout << "Ucitavanje uspjesno" << crt;
	for (auto& biblioteka : biblioteke)
		cout << biblioteka.GetNaziv() << " sa " << biblioteka.GetClanovi().size() << " clanova" << crt;
	cin.get();
	return 0;
}