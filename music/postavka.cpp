/*
	BIG DISCLAIMER

	Ovo je vjezbovni ispitni zadatak, sastavljen po uzoru na stil
	ispita sa fakulteta (Denis Music / Jasmin Azemovic stil zadataka).
	Tematika je izmijenjena - muzicki festival - kako bi se izbjeglo
	ponavljanje prethodno vidjenih zadataka. Zadatak je namjerno malo
	"zabiberen" (regex, threadovi, lambda, operator overloading,
	izuzeci) - u duhu pravih ispita.
*/

#include <iostream>
#include <vector>
#include <regex>
#include <thread>
#include <chrono>
#include <algorithm>
using namespace std;

const char* PORUKA = "\n-------------------------------------------------------------------------------\n"
"0. PROVJERITE DA LI PREUZETI ZADACI PRIPADAJU VASOJ GRUPI (G1/G2)\n"
"1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR\n"
"2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA CE BITI OZNACENO KAO TM\n"
"3. SPASAVAJTE PROJEKAT KAKO BI SE SPRIJECILO GUBLJENJE URADJENOG ZADATKA\n"
"4. ATRIBUTI, NAZIVI METODA (SVE ISTO VAZI I ZA FUNKCIJE), TE BROJ I TIP PARAMETARA MORAJU BITI IDENTICNI "
"ONIMA KOJI SU KORISTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. "
"OSTALE POMOCNE METODE MOZETE IMENOVATI I DODAVATI PO ZELJI.\n"
"5. IZUZETAK BACITE SAMO U METODAMA U KOJIMA JE TO NAZNACENO.\n"
"6. SVE METODE POZVANE U MAIN-U ZADATKA TREBAJU POSTOJATI. UKOLIKO NISTE ZADOVOLJNI IMPLEMENTACIJOM "
"POTREBNO JE DA IMPLEMENTIRATE BAREM TIJELO TIH METODA (METODA MOZE BITI PRAZNA), "
"A AKO METODA TREBA VRATITI NEKI PODATAK ONDA MOZETE VRATITI BILO KOJU TJ. ZELJENU VRIJEDNOST ZAHTIJEVANOG TIPA!\n"
"7. NA KRAJU ISPITA SVOJE RJESENJE KOPIRAJTE U .DOCX FAJL (IMENOVAN BROJEM INDEKSA npr. IB150051.docx)!\n"
"8. RJESENJA ZADATKA POSTAVITE NA FTP SERVER U ODGOVARAJUCI FOLDER!\n"
"9. NEMOJTE POSTAVLJATI VISUAL STUDIO PROJEKTE, VEC SAMO .DOCX FAJL SA VASIM RJESENJEM!\n"
"10. SVE NEDOZVOLJENE RADNJE TOKOM ISPITA CE BITI SANKCIONISANE!\n"
"11. ZA POTREBE TESTIRANJA, U MAIN-U, BUDITE SLOBODNI DODATI TESTNIH PODATAKA (POZIVA METODA) KOLIKO GOD SMATRATE DA JE POTREBNO!\n"
"12. ZA IZRADU ISPITNOG RJESENJA KORISTITI VISUAL STUDIO 2022 I RJESENJE TESTIRAJTE U OBA MODA (F5 i Ctrl+F5)!\n"
"13. NA KRAJU ISPITA PROVJERITE DA LI STE RJESENJE KOPIRALI U ADEKVATAN FOLDER NA FTP SERVERU\n"
"-------------------------------------------------------------------------------\n";

const char* crt = "\n-------------------------------------------\n";

enum Zanr {
	ROK, POP, JAZZ, ELEKTRONSKA, HIP_HOP
};

char* GetNizKaraktera(const char* sadrzaj, bool dealociraj = false) {
	if (sadrzaj == nullptr) return nullptr;
	int vel = strlen(sadrzaj) + 1;
	char* temp = new char[vel];
	strcpy_s(temp, vel, sadrzaj);
	if (dealociraj)
		delete[] sadrzaj;
	return temp;
}

template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;
public:
	Kolekcija() { _trenutno = new int(0); }
	~Kolekcija() {
		delete _trenutno; _trenutno = nullptr;
	}
	void AddElement(T1 e1, T2 e2) {
		_elementi1[*_trenutno] = e1;
		_elementi2[*_trenutno] = e2;
		(*_trenutno)++;
	}
	T1& getElement1(int lokacija) { return _elementi1[lokacija]; }
	T2& getElement2(int lokacija) { return _elementi2[lokacija]; }
	int getTrenutno() const { return *_trenutno; }
	// InsertAt dodaje par vrijednosti na proslijedjenu lokaciju i vraca *this
	Kolekcija& InsertAt(T1 e1, T2 e2, int lokacija) {
		// implementirati pomjeranje elemenata udesno i umetanje na lokaciju
		return *this;
	}
	// RemoveRange uklanja elemente iz opsega [od, do] i vraca objekat tipa Kolekcija
	// koji sadrzi samo uklonjene elemente. Ukoliko opseg nije validan baciti izuzetak.
	Kolekcija RemoveRange(int od, int doIndex) {
		Kolekcija uklonjeni;
		return uklonjeni;
	}
	friend ostream& operator<<(ostream& COUT, Kolekcija& obj) {
		for (int i = 0; i < obj.getTrenutno(); i++)
			COUT << obj.getElement1(i) << " " << obj.getElement2(i) << endl;
		return COUT;
	}
};

class Datum {
	int* _sat, * _minuta, * _sekunda;
public:
	Datum(int sat = 20, int minuta = 0, int sekunda = 0) {
		_sat = new int(sat);
		_minuta = new int(minuta);
		_sekunda = new int(sekunda);
	}
	~Datum() {
		delete _sat; _sat = nullptr;
		delete _minuta; _minuta = nullptr;
		delete _sekunda; _sekunda = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, const Datum& obj) {
		COUT << *obj._sat << ":" << *obj._minuta << ":" << *obj._sekunda;
		return COUT;
	}
};

class Nagrada {
	Datum* _vrijemeUrucenja;
	char* _napomena;
public:
	Nagrada(Datum vrijeme, const char* napomena) {
		_napomena = GetNizKaraktera(napomena);
		_vrijemeUrucenja = new Datum(vrijeme);
	}
	~Nagrada() {
		delete[] _napomena; _napomena = nullptr;
		delete _vrijemeUrucenja; _vrijemeUrucenja = nullptr;
	}
	Datum GetVrijemeUrucenja() { return *_vrijemeUrucenja; }
	char* GetNapomena() { return _napomena; }
	friend ostream& operator<<(ostream& COUT, const Nagrada& obj) {
		COUT << *obj._vrijemeUrucenja << " -> " << obj._napomena;
		return COUT;
	}
};

class Izvodjac {
	static int _id;
	char* _ID; // za inicijalizaciju iskoristiti funkciju GenerisiID na osnovu _imePrezime i statickog clana _id
	char* _imePrezime;
	vector<Nagrada*> _nagrade;
public:
	Izvodjac(const char* imePrezime) {
		_imePrezime = GetNizKaraktera(imePrezime);
		// pozvati GenerisiID i inicijalizovati _ID te inkrementovati _id
	}
	~Izvodjac() {
		delete[] _ID; _ID = nullptr;
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _nagrade.size(); i++) {
			delete _nagrade[i];
			_nagrade[i] = nullptr;
		}
	}
	char* GetImePrezime() { return _imePrezime; }
	char* GetID() { return _ID; }
	vector<Nagrada*>& GetNagrade() { return _nagrade; }
	friend ostream& operator<<(ostream& COUT, Izvodjac& obj) {
		COUT << obj._ID << " -> " << obj._imePrezime << endl;
		for (size_t i = 0; i < obj._nagrade.size(); i++)
			COUT << *obj._nagrade[i] << endl;
		return COUT;
	}
};
int Izvodjac::_id = 0;

class Bend {
	Zanr _zanr;
	vector<Izvodjac> _clanovi;
public:
	Bend(Zanr zanr = ROK) {
		_zanr = zanr;
	}
	Zanr GetZanr() { return _zanr; }
	vector<Izvodjac>& GetClanovi() { return _clanovi; }
	// AddClana onemogucava dodavanje vec postojeceg clana (provjera po ID-u) - baca izuzetak
	void AddClana(Izvodjac clan) {
		_clanovi.push_back(clan);
	}
};

class Festival {
	Kolekcija<Bend*, Bend*, 20> _nastupi;
public:
	Kolekcija<Bend*, Bend*, 20>& GetNastupi() { return _nastupi; }
	// AddNastup onemogucava ponovni "duel" dva ista benda tokom istog festivala - baca izuzetak
	void AddNastup(Bend& b1, Bend& b2) {
	}
	// AddNagrada dodaje nagradu izvodjacu po ID-u ili imenu i prezimenu, unutar konkretnog nastupa
	// dva benda. Onemoguciti dodavanje potpuno iste nagrade (isti izvodjac, isto vrijeme, ista napomena).
	// Nakon uspjesno dodane nagrade, u posebnom thread-u, svim clanovima OBA benda (na email
	// IZVEDEN_IZ_ID@festival2026.com) poslati mail u razmaku od 2 sekunde sa sadrzajem:
	//
	// To: <ID_izvodjaca>@festival2026.com
	// From: info@festival2026.com
	// Subject: Informacija
	// Postovani,
	// U <vrijeme> sati izvodjac <ime i prezime> je osvojio nagradu na ovom nastupu.
	// Trenutni broj nagrada benda je: <broj>
	// Neka pobijedi bolja izvedba.
	bool AddNagrada(Zanr zanrBenda1, Zanr zanrBenda2, const char* idIliImePrezime, Nagrada nagrada) {
		return false;
	}
	friend ostream& operator<<(ostream& COUT, Festival& obj) {
		return COUT;
	}
	// operator() vraca sve izvodjace na festivalu koji imaju broj nagrada >= proslijedjenoj vrijednosti,
	// sortirane opadajuce po broju nagrada (iskoristiti lambda izraz za sortiranje)
	vector<Izvodjac*> operator()(int minBrojNagrada) {
		vector<Izvodjac*> rezultat;
		return rezultat;
	}
};

const char* GetOdgovorNaPrvoPitanje() {
	cout << "Pitanje -> Objasnite razliku izmedju kompozicije i agregacije \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}
const char* GetOdgovorNaDrugoPitanje() {
	cout << "Pitanje -> Objasnite koncept i namjenu smart pointera (unique_ptr, shared_ptr) \n";
	return "Odgovor -> OVDJE UNESITE VAS ODGOVOR";
}

void main() {
	cout << PORUKA;
	cin.get();
	cout << GetOdgovorNaPrvoPitanje() << endl;
	cin.get();
	cout << GetOdgovorNaDrugoPitanje() << endl;
	cin.get();

	/*
	Globalna funkcija GenerisiID vraca ID izvodjaca na osnovu vrijednosti
	proslijedjenih parametara. Funkcija generise ID sa maksimalno 4 cifre,
	ne racunajuci inicijale tj. prva slova imena i prezimena.
	Podrazumijeva se da ce uvijek biti proslijedjene validne vrijednosti
	parametara. Ukoliko se proslijede mala pocetna slova imena ili prezimena,
	potrebno ih je konvertovati u velika.
	U okviru generisanog ID-a, proslijedjena cjelobrojna vrijednost treba
	biti u obrnutom redoslijedu cifara.
	*/
	cout << GenerisiID("Dino Merlin", 7) << endl;      //treba vratiti D000M7
	cout << GenerisiID("Hari Varesanovic", 25) << endl; //treba vratiti H00H52
	cout << GenerisiID("Amira Medunjanin", 312) << endl;//treba vratiti A0M213
	cout << GenerisiID("edo maajka", 4089) << endl;     //treba vratiti EM9804

	// Za validaciju ID-a koristiti funkciju ValidirajID koja treba, koristeci regex,
	// osigurati postivanje pravila navedenih u prethodnom dijelu zadatka.
	if (ValidirajID("D000M7"))
		cout << "ID VALIDAN" << endl;
	if (ValidirajID("H00H52"))
		cout << "ID VALIDAN" << endl;
	if (!ValidirajID("A00M213"))
		cout << "ID NIJE VALIDAN" << endl;
	if (!ValidirajID("em9804"))
		cout << "ID NIJE VALIDAN" << endl;

	int kolekcijaTestSize = 9;
	Kolekcija<int, int, 10> kolekcija1;
	for (int i = 0; i < kolekcijaTestSize; i++)
		kolekcija1.AddElement(i, i); // dodaje vrijednosti u kolekciju
	cout << kolekcija1 << crt;

	/* metoda InsertAt treba da doda vrijednosti prvog i drugog parametra
	na lokaciju koja je definisana trecim parametrom. Povratna vrijednost
	metode je objekat (pozivaoc metode, u konkretnom slucaju kolekcija1) u
	okviru koga su, na definisanu lokaciju, dodati zahtijevani parametri.
	Nakon izvrsenja metode InsertAt, oba objekta, kolekcija1 i kolekcija2,
	bi trebali posjedovati sljedeci sadrzaj:
	10 10
	0 0
	1 1
	2 2
	....
	*/
	Kolekcija<int, int, 10> kolekcija2 = kolekcija1.InsertAt(10, 10, 0);
	cout << kolekcija2 << crt;

	/* Metoda RemoveRange prihvata lokaciju OD i DO, te u tom opsegu uklanja
	sve elemente iz kolekcije. U slucaju da zahtijevani opseg ne postoji u
	kolekciji, metoda treba baciti izuzetak. Na kraju, metoda treba vratiti
	novi objekat tipa Kolekcija koji sadrzi samo uklonjene elemente. */
	Kolekcija<int, int, 10> kolekcija3 = kolekcija1.RemoveRange(1, 3);
	cout << kolekcija3 << endl;
	cout << kolekcija1 << crt;
	/* kolekcija3 bi trebala sadrzavati sljedece elemente:
	0 0
	1 1
	2 2
	dok bi kolekcija1 trebala sadrzavati sljedece elemente:
	10 10
	3 3
	4 4
	......
	*/
	kolekcija1 = kolekcija3;
	cout << kolekcija1;

	Datum
		prvaNagrada203115(20, 31, 15),
		drugaNagrada204502(20, 45, 2),
		trecaNagrada211809(21, 18, 9),
		cetvrtaNagrada213327(21, 33, 27);

	Izvodjac dino("Dino Merlin"), hari("Hari Varesanovic"),
		amira("Amira Medunjanin"), edo("Edo Maajka");

	if (strcmp(dino.GetID(), "D000M1") == 0 && strcmp(hari.GetID(), "H000H2") == 0)
		cout << "ID se uspjesno generise!" << endl;

	Nagrada prvaN(prvaNagrada203115, "publika ustala na noge"),
		drugaN(drugaNagrada204502, "najbolji vokal vecери"),
		trecaN(trecaNagrada211809, "najduzi aplauz na festivalu"),
		cetvrtaN(cetvrtaNagrada213327, "specijalno priznanje zirija");

	Bend rokBend(ROK), popBend(POP);
	rokBend.AddClana(dino);
	rokBend.AddClana(hari);
	popBend.AddClana(amira);
	popBend.AddClana(edo);

	try
	{
		rokBend.AddClana(dino); // onemoguciti dodavanje istog izvodjaca - provjeravati ID
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}

	Festival muzickiFestival2026;
	muzickiFestival2026.AddNastup(rokBend, popBend);
	try
	{
		muzickiFestival2026.AddNastup(rokBend, popBend); // onemoguciti ponovni "duel" istih bendova
	}
	catch (exception& obj)
	{
		cout << obj.what();
	}

	// omoguciti dodavanje nagrade po ID-u ili imenu i prezimenu
	if (muzickiFestival2026.AddNagrada(ROK, POP, "D000M1", prvaN))
		cout << "Nagrada uspjesno dodata" << endl;
	// onemoguciti dodavanje potpuno iste nagrade
	if (!muzickiFestival2026.AddNagrada(ROK, POP, "Dino Merlin", prvaN))
		cout << "Nagrada NIJE uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "H000H2", drugaN))
		cout << "Nagrada uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "Hari Varesanovic", trecaN))
		cout << "Nagrada uspjesno dodata" << endl;
	if (muzickiFestival2026.AddNagrada(ROK, POP, "Amira Medunjanin", cetvrtaN))
		cout << "Nagrada uspjesno dodata" << endl;

	// ispisuje detaljnije informacije o nastupu, kako je navedeno u narednom ispisu
	cout << muzickiFestival2026;
	/*
	-------------------------------------------
	(3) ROK : POP (1)
	-------------------------------------------
	Dino Merlin        Amira Medunjanin
	Hari Varesanovic
	Hari Varesanovic
	-------------------------------------------
	*/

	// vraca sve izvodjace koji su na festivalu osvojili broj nagrada koji je >= proslijedjenoj vrijednosti,
	// sortirane opadajuce po broju nagrada
	vector<Izvodjac*> izvodjaci = muzickiFestival2026(2);
	for (size_t i = 0; i < izvodjaci.size(); i++)
		cout << izvodjaci[i]->GetImePrezime() << endl;

	cin.get();
	system("pause>0");
}