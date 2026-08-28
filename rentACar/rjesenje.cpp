#include <iostream>
#include <vector>
#include <string>
#include <regex>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
using namespace std;

string crt = "\n------------------------------------------------------ -------------------------- - \n";

char* AlocirajTekst(const char* tekst) {
	if (!tekst) return nullptr;
	size_t vel = strlen(tekst) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, tekst);
	return temp;
}
int getTrenutnuGodinuIzSistema() {
	time_t trenutnoVrijeme = time(nullptr);
	tm timeInfo{};
	localtime_s(&timeInfo, &trenutnoVrijeme);
	int godina = (timeInfo.tm_year + 1900);
	return godina;
}
// funkcija generise sifru klijenta na osnovu imena i prezimena, rednog broja i trenutne
// godine. sifra je u formatu RC-IN-BBB/GGGG, gdje IN predstavlja inicijale, BBB redni broj
// klijenta popunjen nulama na slobodnim mjestima, a GGGG trenutnu godinu dobijenu iz sistema.
// funkciju koristiti u konstruktoru klase Klijent za inicijalizaciju atributa _sifra.
string GenerisiSifru(const char* imePrezime, int redniBroj) {
	string sifra;

	sifra += "RC-";
	sifra += toupper(imePrezime[0]);
	const char* prezime = strchr(imePrezime, ' ');
	if (prezime != nullptr && strlen(prezime) > 1) sifra += toupper(prezime[1]);
	sifra += "-";

	if (redniBroj < 10) sifra += "00";
	else if (redniBroj < 100) sifra += "0";
	sifra += to_string(redniBroj);
	sifra += "/";

	int godina = getTrenutnuGodinuIzSistema();
	sifra += to_string(godina);

	return sifra;
};
// koristeci regex, funkcija ValidirajSifru provjerava da li je sifra zapisana u prethodno
	// definisanom formatu. funkcija vraca true ako je sifra validna, u suprotnom vraca false.
bool ValidirajSifru(const string& sifra) {
	return regex_match(sifra, regex("RC-[A-Z]{2}-[0-9]{3}/2026"));
};

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
	const T1& GetPrvi(int indeks) const { return _prvi[indeks]; }
	const T2& GetDrugi(int indeks) const { return _drugi[indeks]; }
	T1& operator[](int indeks) { return _prvi[indeks]; }
	Kolekcija(const Kolekcija& obj) {
		_trenutno = new int(obj.GetTrenutno());

		for (size_t i = 0; i < *_trenutno; i++)
		{
			_prvi[i] = obj.GetPrvi(i);
			_drugi[i] = obj.GetDrugi(i);
		}
	}
	Kolekcija& operator=(const Kolekcija& obj) {
		if (this != &obj)
		{
			delete _trenutno; _trenutno = nullptr;

			_trenutno = new int(obj.GetTrenutno());

			for (size_t i = 0; i < *_trenutno; i++)
			{
				_prvi[i] = obj.GetPrvi(i);
				_drugi[i] = obj.GetDrugi(i);
			}
		}
		return *this;
	}
	// DodajNaPoziciju dodaje novi par na lokaciju definisanu prvim parametrom, pomjera
	// postojece elemente udesno i vraca trenutno stanje kolekcije (*this). U slucaju
	// popunjene kolekcije ili neispravne lokacije potrebno je baciti izuzetak.
	Kolekcija DodajNaPoziciju(int pozicija, const T1& el1, const T2& el2) {
		if (pozicija < 0 || pozicija > *_trenutno) throw exception("Pozicija nije validna");
		if (*_trenutno >= max) throw exception("Kolekcija puna");
		for (int i = *_trenutno; i > pozicija; i--)
		{
			_prvi[i] = _prvi[i - 1];
			_drugi[i] = _drugi[i - 1];
		}

		_prvi[pozicija] = el1;
		_drugi[pozicija] = el2;

		(*_trenutno)++;
		return *this;
	}
	// UkloniRaspon od lokacije definisane prvim parametrom uklanja broj elemenata definisan
	// drugim parametrom, a vraca pokazivac na novu kolekciju koja sadrzi uklonjene elemente.
	// Pozivalac je odgovoran za dealokaciju vracene kolekcije. Za neispravan raspon baciti izuzetak.
	Kolekcija* UkloniRaspon(int lokacija, int brojElemenata) {
		Kolekcija* temp = new Kolekcija();
		for (size_t i = lokacija; i < lokacija + brojElemenata; i++)
		{
			temp->Dodaj(_prvi[i], _drugi[i]);
		}
		for (size_t i = lokacija + brojElemenata; i < *_trenutno; i++)
		{
			_prvi[i - brojElemenata] = _prvi[i];
			_drugi[i - brojElemenata] = _drugi[i];
		}
		*_trenutno -= brojElemenata;
		return temp;
	}
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.GetTrenutno(); i++)
			COUT << obj.GetPrvi(i) << " " << obj.GetDrugi(i) << endl;
		return COUT;
	}

	void Dodaj(const T1& el1, const T2& el2) {
		if (*_trenutno >= max) return;

		_prvi[*_trenutno] = el1;
		_drugi[*_trenutno] = el2;

		(*_trenutno)++;
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
	DatumVrijeme(const DatumVrijeme& obj) {
		_godina = new int(*obj._godina);
		_mjesec = new int(*obj._mjesec);
		_dan = new int(*obj._dan);
		_sati = new int(*obj._sati);
		_minute = new int(*obj._minute);
	}
	DatumVrijeme& operator=(const DatumVrijeme& obj) {
		if (this != &obj)
		{
			delete _godina; delete _mjesec; delete _dan;
			delete _sati; delete _minute;

			_godina = new int(*obj._godina);
			_mjesec = new int(*obj._mjesec);
			_dan = new int(*obj._dan);
			_sati = new int(*obj._sati);
			_minute = new int(*obj._minute);
		}
		return *this;
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

	const int getDan() const { return *_dan; };
	const int getMjesec() const { return *_mjesec; };
	const int getGodina() const { return *_godina; };
	const int getSati() const { return *_sati; };
	const int getMinute() const { return *_minute; };

	bool daLiJeIstiDan(const DatumVrijeme& obj) const {
		if (obj.getDan() == *_dan && obj.getMjesec() == *_mjesec && obj.getGodina() == *_godina)return true;
		return false;
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
	Rezervacija(const Rezervacija& obj) {
		_registarskaOznaka = AlocirajTekst(obj.GetRegistarskaOznaka());
		_tipVozila = obj.GetTipVozila();
		_pocetak = obj.GetPocetak();
		_trajanjeSati = obj.GetTrajanjeSati();
	}
	Rezervacija& operator=(const Rezervacija& obj) {
		if (this != &obj)
		{
			delete[] _registarskaOznaka; _registarskaOznaka = nullptr;

			_registarskaOznaka = AlocirajTekst(obj.GetRegistarskaOznaka());
			_tipVozila = obj.GetTipVozila();
			_pocetak = obj.GetPocetak();
			_trajanjeSati = obj.GetTrajanjeSati();
		}
		return *this;
	}
	~Rezervacija() { delete[] _registarskaOznaka; _registarskaOznaka = nullptr; }
	const char* GetRegistarskaOznaka() const { return _registarskaOznaka; }
	TipVozila GetTipVozila() const { return _tipVozila; }
	DatumVrijeme& GetPocetak() { return _pocetak; }
	const DatumVrijeme& GetPocetak() const { return _pocetak; }
	int GetTrajanjeSati() const { return _trajanjeSati; }
	// ToString vraca podatke o rezervaciji u formatu:
	// 10.09.2026 09:00 ABC-123 SUV 5h
	// voditi racuna o prikazu jednocifrenih vrijednosti datuma i vremena (npr. 9 -> 09)
	string ToString() {
		stringstream ss;

		ss << setw(2) << setfill('0') << GetPocetak().getDan() << "."
			<< setw(2) << setfill('0') << GetPocetak().getMjesec() << "."
			<< setw(2) << setfill('0') << GetPocetak().getGodina() << " "
			<< setw(2) << setfill('0') << GetPocetak().getSati() << ":"
			<< setw(2) << setfill('0') << GetPocetak().getMinute() << " ";

		ss << GetRegistarskaOznaka() << " " << TipVozilaNazivi[GetTipVozila()] << " " << GetTrajanjeSati() << "h";
		return ss.str();
	}
	// ImaKonfliktSa vraca true ako su rezervacije istog datuma i njihovi vremenski intervali
	// se preklapaju. rezervacija koja pocinje u trenutku kada prethodna zavrsava nije konfliktna.
	bool ImaKonfliktSa(const Rezervacija& rez) {
		if (!GetPocetak().daLiJeIstiDan(rez.GetPocetak())) return false;

		int pocetakPrve = GetPocetak().getSati() * 60 + GetPocetak().getMinute();
		int krajPrve = pocetakPrve + _trajanjeSati * 60;

		int pocetakDruge = rez.GetPocetak().getSati() * 60 + rez.GetPocetak().getMinute();
		int krajDruge = pocetakDruge + rez.GetTrajanjeSati() * 60;

		if (krajPrve <= pocetakDruge || krajDruge <= pocetakPrve) return false;
		return true;
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
	Klijent(const Klijent& obj) {
		_sifra = AlocirajTekst(obj.GetSifra());
		_imePrezime = AlocirajTekst(obj.GetImePrezime());
		_rezervacije = obj._rezervacije;
	}
	Klijent& operator=(const Klijent& obj) {
		if (this != &obj)
		{
			delete[] _sifra; _sifra = nullptr;
			delete[] _imePrezime; _imePrezime = nullptr;

			_sifra = AlocirajTekst(obj.GetSifra());
			_imePrezime = AlocirajTekst(obj.GetImePrezime());
			_rezervacije = obj._rezervacije;
		}
		return *this;
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
	bool DodajRezervaciju(const Rezervacija& rezervacija) {
		for (size_t i = 0; i < _rezervacije.size(); i++)
		{
			if (_rezervacije[i].ImaKonfliktSa(rezervacija))return false;
		}

		int ukupnoTrajanje = rezervacija.GetTrajanjeSati() * 60;
		for (size_t i = 0; i < _rezervacije.size(); i++)
		{
			if (_rezervacije[i].GetPocetak().daLiJeIstiDan(rezervacija.GetPocetak())) {
				ukupnoTrajanje += _rezervacije[i].GetTrajanjeSati() * 60;
			}
		}

		if (ukupnoTrajanje > 720) return false;

		_rezervacije.push_back(rezervacija);
		return true;
	}
	friend ostream& operator<<(ostream& COUT, Klijent& obj) {
		COUT << obj._imePrezime << " [" << obj._sifra << "]" << endl;
		for (auto& rezervacija : obj._rezervacije)
			COUT << " - " << rezervacija.ToString() << endl;
		return COUT;
	}

	bool operator==(const Klijent& obj) const {
		return strcmp(obj.GetSifra(), _sifra) == 0;
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
	RentACarAgencija& operator=(const RentACarAgencija& obj) {
		if (this != &obj)
		{
			delete[] _naziv; _naziv = nullptr;

			_naziv = AlocirajTekst(obj._naziv);
			_klijenti = obj._klijenti;
		}
		return *this;
	}
	const char* GetNaziv() const { return _naziv; }
	vector<Klijent>& GetKlijenti() { return _klijenti; }
	// DodajKlijenta onemogucava dodavanje klijenta sa istom sifrom i baca izuzetak
	void DodajKlijenta(const Klijent& klijent) {
		for (const auto& k : _klijenti) {
			if (k == klijent) throw exception("Klijent vec dodan");
		}
		_klijenti.push_back(klijent);
	}
	
	// RegistrujRezervaciju pronalazi klijenta na osnovu sifre i dodaje mu rezervaciju.
	// i dalje vaze pravila definisana u metodi DodajRezervaciju. metoda vraca true ili false.
	bool RegistrujRezervaciju(const char* sifra, const Rezervacija& rezervacija) {
		for (size_t i = 0; i < _klijenti.size(); i++)
		{
			if (strcmp(_klijenti[i].GetSifra(), sifra) == 0) {
				return _klijenti[i].DodajRezervaciju(rezervacija); 
			}
		}
		return false;
	}
	// AktivniKlijenti vraca pokazivace na klijente koji imaju najmanje onoliko rezervacija
	// koliko je definisano vrijednoscu proslijedjenog parametra.
	vector<Klijent*> AktivniKlijenti(int minRezervacija) {
		vector<Klijent*> temp;
		for (size_t i = 0; i < _klijenti.size(); i++)
		{
			if (_klijenti[i].GetRezervacije().size() >= minRezervacija) temp.push_back(&_klijenti[i]);
		}
		return temp;
	}
	// KoristenjePoTipu vraca kolekciju parova (klijent, broj sati) za sve klijente koji
	// imaju najmanje jednu rezervaciju vozila proslijedjenog tipa.
	Kolekcija<Klijent, int, 50> KoristenjePoTipu(TipVozila tip) {
		Kolekcija<Klijent, int, 50> temp;

		for (size_t i = 0; i < _klijenti.size(); i++)
		{
			int ukupnoSati = 0;
			for (size_t j = 0; j < _klijenti[i].GetRezervacije().size(); j++)
			{
				Rezervacija& r = _klijenti[i].GetRezervacije()[j];
				if (r.GetTipVozila() == tip) {
					ukupnoSati += r.GetTrajanjeSati();
				}
			}
			if (ukupnoSati > 0)
			{
				temp.Dodaj(_klijenti[i], ukupnoSati);
			}
		}
		return temp;
	}
};

bool UcitajPodatke(const string& putanja, vector<RentACarAgencija>& ispostave)
{
	fstream fajl{ putanja };
	if (!fajl.is_open()) return false;

	string nazivIspostave;
	string nazivKlijenta;

	bool ucitan = false;

	while (getline(fajl, nazivIspostave, '|') && getline(fajl, nazivKlijenta))
	{
		RentACarAgencija* trenutnaIspostava = nullptr;
		for (auto& i : ispostave) {
			if (i.GetNaziv() == nazivIspostave) {
				trenutnaIspostava = &i;
				break;
			}
		}
		if (trenutnaIspostava == nullptr)
		{
			ispostave.push_back(RentACarAgencija(nazivIspostave.c_str()));
			trenutnaIspostava = &ispostave.back();
		}

		bool postoji = false;

		for (auto& k : trenutnaIspostava->GetKlijenti()) {
			if (k.GetImePrezime() == nazivKlijenta) {
				postoji = true;
				break;
			}
		}
		if (!postoji)
		{
			trenutnaIspostava->DodajKlijenta(Klijent(nazivKlijenta.c_str()));
			ucitan = true;
		}
	}
	return ucitan;
}

int main() {
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

	Kolekcija<int, string, 20>* uklonjeniTermini = prosireniTermini.UkloniRaspon(3, 4);
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
	if (!auto1.ImaKonfliktSa(auto4))
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