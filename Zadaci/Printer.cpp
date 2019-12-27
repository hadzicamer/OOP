#include <iostream>
#include<vector>
#include<string>
#include<iterator>
#include<regex>
#include<thread>
#include<mutex>
using namespace std;

const char* crt = "\n---------------------------------------\n";

class DatumVrijeme
{
	int* _dan, * _mjesec, * _godina, * _sati, * _minuti;
public:
	DatumVrijeme(int dan = 1, int mjesec = 1, int godina = 2000, int sati = 0, int minuti = 0)
	{
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
		_sati = new int(sati);
		_minuti = new int(minuti);
	}

	DatumVrijeme(const DatumVrijeme& dat) {
		_dan = new int(*dat._dan);
		_mjesec = new int(*dat._mjesec);
		_godina = new int(*dat._godina);
		_sati = new int(*dat._sati);
		_minuti = new int(*dat._minuti);
	}

	~DatumVrijeme()
	{
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
		delete _sati; _sati = nullptr;
		delete _minuti; _minuti = nullptr;
	}
	friend ostream& operator<< (ostream& COUT, DatumVrijeme& obj)
	{
		COUT << *obj._dan << "." << *obj._mjesec << "." << *obj._godina << " " << *obj._sati << ":" << *obj._minuti;
		return COUT;
	}

	void operator=(const DatumVrijeme& dat) {
		if (this != &dat) {
			*_dan = *dat._dan;
			*_mjesec= *dat._mjesec;
			*_godina = *dat._godina;
			*_sati= *dat._sati;
			*_minuti = *dat._minuti;
		}
	}

	bool operator==(const DatumVrijeme& d) {
		return *_dan == *d._dan && *_mjesec == *d._mjesec && *_godina == *d._godina && *_sati == *d._sati;
	}
};

template<class T1, class T2 = int>
class FITKolekcija
{
	T1* _elementi1;
	T2* _elementi2;
	int _trenutno;
public:
	FITKolekcija()
	{
		_elementi1 = nullptr;
		_elementi2 = nullptr;
		_trenutno = 0;
	}
	FITKolekcija(const FITKolekcija& kol) {
		_trenutno = kol._trenutno;
		_elementi1 = new T1[kol._trenutno];
		_elementi2 = new T2[kol._trenutno];
		for (size_t i = 0; i < _trenutno; i++)
		{
			_elementi1[i] = kol._elementi1[i];
			_elementi2[i] = kol._elementi2[i];
		}
	}
	~FITKolekcija()
	{
		delete[] _elementi1; _elementi1 = nullptr;
		delete[] _elementi2; _elementi2 = nullptr;
	}
	T1* GetT1()const { return _elementi1; }
	T2* GetT2()const { return _elementi2; }
	int GetTrenutno() const { return _trenutno; }

	void Dodaj(T1 el1, T2 el2) {

		T1* temp1 = new T1[_trenutno + 1];
		T2* temp2 = new T2[_trenutno + 1];
		for (size_t i = 0; i < _trenutno; i++)
		{
			temp1[i] = _elementi1[i];
			temp2[i] = _elementi2[i];
		}
		delete[]_elementi1;
		delete[]_elementi2;
		temp1[_trenutno] = el1;
		temp2[_trenutno] = el2;
		_elementi1 = temp1;
		_elementi2 = temp2;
		_trenutno++;
	}

	void Sortiraj() {

		bool sort = true;
		while (sort)
		{
			sort = false;
			for (size_t i = 0; i < _trenutno - 1; i++)
			{
				if (_elementi1[i] > _elementi1[i + 1])
				{
					T1 temp1 = _elementi1[i];
					T2 temp2 = _elementi2[i];
					_elementi1[i] = _elementi1[i + 1];
					_elementi2[i] = _elementi2[i + 1];
					_elementi1[i + 1] = temp1;
					_elementi2[i + 1] = temp2;
					sort = true;
				}
			}
		}
	}

	bool DodajNaLokaciju(T1 el1, T2 el2, int indeks) {
		if (indeks < 0 || indeks >= _trenutno)
			return false;

		Dodaj(el1, el2);
		swap(_elementi1[_trenutno - 1], _elementi1[indeks]); // zamjeni zadnji i element na drugoj poziciji
		swap(_elementi2[_trenutno - 1], _elementi2[indeks]);
		return true;
	}

	FITKolekcija<int, int>* GetJedinstveni() {
		FITKolekcija<int, int>* el = new FITKolekcija<int, int>;
		
		for (size_t i = 0; i < _trenutno; i++)
		{
			bool dodaj = false;
			for (size_t j = 0; j < el->GetTrenutno(); j++)
			{
				if (_elementi1[i] == el->GetT1()[j] && _elementi2[i] == el->GetT2()[j])
					dodaj = true;
			}
			if (!dodaj)
				el->Dodaj(_elementi1[i], _elementi2[i]);
		}
		return el;
	}

	void operator=(const FITKolekcija& kol) {
		if (this != &kol) {
			delete[]_elementi1; 
			delete[]_elementi2;
			_trenutno = kol._trenutno;
			_elementi1= new T1[kol._trenutno];
			_elementi2 = new T2[kol._trenutno];
			for (size_t i = 0; i < _trenutno; i++)
			{
				_elementi1[i] = kol._elementi1[i];
				_elementi2[i] = kol._elementi2[i];
			}
		}
	}

	friend ostream& operator<< (ostream& COUT, FITKolekcija& obj)
	{
		for (size_t i = 0; i < obj._trenutno; i++)
			COUT << obj._elementi1[i] << " " << obj._elementi2[i] << endl;
		return COUT;
	}
};

enum vrstaDokumenta { PDF, DOC, TXT, HTML };

class Dokument
{
	vrstaDokumenta _vrsta;
	string _naziv;
	string _sadrzaj;
	int _brojStranica;
public:
	Dokument() { _brojStranica = 0; _naziv = ""; _sadrzaj = ""; }

	Dokument(vrstaDokumenta vrsta, string naziv)
	{
		_naziv = naziv; _vrsta = vrsta; _brojStranica = 0;
	}
	string GetNaziv() { return _naziv; };
	string GetSadrzaj() { return _sadrzaj; };
	vrstaDokumenta GetVrsta() { return _vrsta; };
	void UvecajBrojStranica() { _brojStranica++; }
	int GetBrojStranica() { return _brojStranica; }
	
	void DodajSadrzaj(string sadrzaj) {
		_sadrzaj += sadrzaj;
		_brojStranica = _sadrzaj.length() / 30;
	}
	
	friend ostream& operator<<(ostream& COUT, Dokument& obj)
	{
		COUT << obj._naziv << "." << obj._vrsta << " (" << obj._brojStranica << ")\n" << obj._sadrzaj << endl;
		return COUT;
	}
};

class Uredjaj
{
protected:
	char* _proizvodjac;
	char* _model;
public:
	Uredjaj(const char* proizvodjac, const char* model)
	{
		int vel = strlen(proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, proizvodjac);
		vel = strlen(model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, model);
	}

	Uredjaj(const Uredjaj& ur) {
		int vel = strlen(ur._proizvodjac) + 1;
		_proizvodjac = new char[vel];
		strcpy_s(_proizvodjac, vel, ur._proizvodjac);
		vel = strlen(ur._model) + 1;
		_model = new char[vel];
		strcpy_s(_model, vel, ur._model);
	}

	~Uredjaj()
	{
		delete[] _proizvodjac; _proizvodjac = nullptr;
		delete[] _model; _model = nullptr;
	}
	friend ostream& operator<<(ostream& COUT, Uredjaj& obj)
	{
		COUT << obj._proizvodjac << "." << obj._model << endl;
		return COUT;
	}

};

mutex m;
void print(string text) {
	lock_guard<mutex>guard(m);
	cout << text << endl;
}
class Printer:public Uredjaj
{
	FITKolekcija<DatumVrijeme, Dokument> _printaniDokumenti;
	vector<string> _zabranjeneRijeci;
public:
	Printer(const char* proizv, const char* model) :Uredjaj(proizv, model) {}; // ODE LI U NULLPTR SIGURNO SAM PROSLIJEDIO PRAZNE A NE PROSLIJEDENE VRIJEDNOSTI
	Printer(const Printer& pri) :Uredjaj(pri._proizvodjac, pri._model), _printaniDokumenti(pri._printaniDokumenti), _zabranjeneRijeci(pri._zabranjeneRijeci) {};
	
	FITKolekcija<DatumVrijeme, Dokument>& GetPrintaniDokumenti() { return _printaniDokumenti; };
	vector<string>& GetZabranjeneRijeci() { return _zabranjeneRijeci; };
	
	void DodajZabranjenuRijec(string rijec) {
		for (vector<string>::iterator i = _zabranjeneRijeci.begin(); i != _zabranjeneRijeci.end(); i++)
		{
			if (*i == rijec)
				return;
		}
		_zabranjeneRijeci.push_back(rijec);
	}

	void Printaj(DatumVrijeme dat, Dokument doku) {
		_printaniDokumenti.Dodaj(dat, doku);
		if (!regex_match(doku.GetNaziv(), regex("([A-Za-z]{5,15})([.])(pdf|doc|html|txt)")))
			throw exception("Naziv dokumenta nije u odgovarajucem formatu");
		for (vector<string>::iterator i = _zabranjeneRijeci.begin(); i != _zabranjeneRijeci.end(); i++)
		{
			if (doku.GetSadrzaj().find(*i) != string::npos)
				throw exception("Sadrzaj dokumenta sadrzi zabranjenu/e rijeci/i");
		}

		thread t(print, doku.GetSadrzaj());
		t.detach();
	}

	int GetProsjecanBrojStranicaPoDatumu(DatumVrijeme dat) {
		float ukupno = 0, printanihStr = 0;

		for (size_t i = 0; i < _printaniDokumenti.GetTrenutno(); i++)
		{
			if (_printaniDokumenti.GetT1()[i] == dat) {
				bool print = true;
				if (!regex_match(_printaniDokumenti.GetT2()[i].GetNaziv(), regex("([A-Za-z]{5,15})([.])(pdf|doc|html|txt)")))
					print = false;
				for (vector<string>::iterator j = _zabranjeneRijeci.begin(); j != _zabranjeneRijeci.end(); j++)
				{
					if (_printaniDokumenti.GetT2()[i].GetNaziv().find(*j) != string::npos)
						print = true;
					break;
				}
			}
			if (print) {
				ukupno += _printaniDokumenti.GetT2()[i].GetBrojStranica();
				printanihStr = _printaniDokumenti.GetT2()[i].GetBrojStranica();
			}
			else
				ukupno += _printaniDokumenti.GetT2()[i].GetBrojStranica();
		}
		if (ukupno > 0)
			return printanihStr/ukupno;
		return 0;
	}

	friend ostream& operator<<(ostream& cout, Printer& pri) {
		cout << *dynamic_cast<Uredjaj*>(&pri) << endl;
		cout << "PRINTANI DOKUMENTI: " << endl;
		for (size_t i = 0; i < pri._printaniDokumenti.GetTrenutno(); i++)
			cout << pri._printaniDokumenti.GetT2()[i].GetNaziv() << endl;
		cout << endl;
		cout << "ZABRANJENE RIJECI" << endl;

		for (vector<string>::iterator j = pri._zabranjeneRijeci.begin(); j != pri._zabranjeneRijeci.end(); j++)
					cout << *j << endl;
		return cout;
	}
};

void main()
{
	/****************************************************************************
	// 1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	// 2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR
	//    ÆE BITI OZNACENO KAO "RE"
	// 3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEÈILO GUBLJENJE URAÐENOG ZADATKA
	// 4. PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT
	// 5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIÈNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U,
	//    OSIM U SLUÈAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOÆNE FUNKCIJE MOŽETE
	//    IMENOVATI I DODAVATI PO ŽELJI.
	// 6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAÈENO.
	****************************************************************************/
#pragma region TestiranjeDatumVrijeme
	DatumVrijeme danas(1, 2, 2017, 10, 15);
	DatumVrijeme sutra(danas);
	DatumVrijeme prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
#pragma endregion
#pragma region FITKolekcija
	int v6 = 6, v13 = 13, v32 = 32, v63 = 63, v98 = 98, v109 = 109, v196 = 196;

	FITKolekcija<int, int> brojevi;
	brojevi.Dodaj(v196, v6);
	brojevi.Dodaj(v13, v32);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v63, v13);
	brojevi.Dodaj(v98, v196);
	brojevi.Dodaj(v196, v6);

	cout << brojevi << crt;
	////SORTIRANJE ÈLANOVA KOLEKCIJE SE VRŠI U RASTUÆEM REDOSLIJEDU NA OSNOVU VRIJEDNOSTI ELEMENTA TIPA T1
	brojevi.Sortiraj();
	cout << brojevi << crt;
	////BROJ 2 SE ODNOSI NA LOKACIJU/INDEKS UNUTAR LISTE NA KOJI JE POTREBNO DODATI NOVE ELEMENTE
	brojevi.DodajNaLokaciju(v109, v6, 2);
	cout << brojevi << crt;
	brojevi.Sortiraj();
	cout << brojevi << crt;
	// METODA GetJedinstveni VRAÆA LISTU JEDINSTVENIH ELEMENATA TJ. ELEMENATA KOJI NE SADRŽE DUPLIKATE 
	// (POJAM DUPLIKAT SE ODNOSI NA ISTE VRIJEDNOSTI ELEMENATA T1 I T2). ELEMENTI KOJI SE DUPLIRAJU SE U 
	// OVOJ LISTI TREBAJU POJAVITI SAMO JEDNOM.
	FITKolekcija<int, int>* jedinstveni = brojevi.GetJedinstveni();
	cout << *jedinstveni << crt;
	*jedinstveni = brojevi;
	cout << *jedinstveni << crt;

	Dokument ispitPRIII(DOC, "ispitPRIII.doc");
	////BROJ STRANICA DOKUMENTA SE AUTOMATSKI ODREÐUJE PRILIKOM DODAVANJA SADRŽAJA. ZA POTREBE ISPITA PRETPOSTAVLJAMO
	////DA NA JEDNU STRANICU MOŽE STATI 30 ZNAKOVA UKLJUÈUJUÆI I RAZMAKE
	ispitPRIII.DodajSadrzaj("NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ÆE BITI OZNACENO KAO RE");
	cout << "Broj stranica -> " << ispitPRIII.GetBrojStranica() << endl;
	Dokument ispitBaze(DOC, "ispitBaze.doc");
	ispitBaze.DodajSadrzaj("PROGRAMSKI CODE SE TAKOÐER NALAZI U FAJLU CODE.TXT");
	cout << ispitBaze << endl;//ISPISUJE SVE DOSTUPNE PODATKE O DOKUMENTU

	Printer hp3200("HP", "3200");
	////PRINTER NEÆE DOZVOLITI PRINTANJE DOKUMENATA U ÈIJEM SADRŽAJU SE NALAZI NEKA OD ZABRANJENIH RIJEÈI
	hp3200.DodajZabranjenuRijec("RE");// :)
	hp3200.DodajZabranjenuRijec("RAT");
	hp3200.DodajZabranjenuRijec("UBITI");
	hp3200.DodajZabranjenuRijec("RE");// ONEMOGUÆITI PONAVLJANJE ZABRANJENIH RIJEÈI, KORISTITI ITERATORE
	hp3200.DodajZabranjenuRijec("MRZITI");
	try
	{
		// DA BI PRINTER ISPRINTAO NEKI DOKUMENT MORAJU BITI ZADOVOLJENA SLJEDEÆA PRAVILA:
		// 1. NAZIV DOKUMENTA MORA BITI U SLJEDEÆEM FORMATU npr: ispitPRIII.doc
		// NAZIV DOKUMENTA MOŽE SADRŽAVATI SAMO SLOVA (NAJMANJE 5, NAJVIŠE 15), A EKSTENZIJA MOŽE 
		// BITI SAMO NEKA OD DOZVOLJENIH VRSTOMDOKUMENTA
		// 2. SADRŽAJ DOKUMENTA U SEBI NE SMIJE POSJEDOVATI NITI JEDNU ZABRANJENU RIJEÈ
		// UKOLIKO NEKI OD NAVEDENIH USLOVA NIJE ZADOVOLJEN FUNKCIJA TREBA DA BACI EXCEPTION SA 
		// ODGOVARAJUÆOM PORUKOM, ALI ÆE DOKUMENT BITI POHRANJEN U KOLEKCIJU _printaniDokumenti. 
		// UKOLIKO DOKUMENT ISPUNJAVA SVE USLOVE ZA PRINTANJE POTREBNO JE NA KONZOLU ISPISATI 
		// SADRŽAJ DOKUMENTA KOJI SE PRINTA

		hp3200.Printaj(danas, ispitPRIII);
		hp3200.Printaj(danas, ispitBaze);


		// IMPLEMENTIRATI PRINTANJE U ZASEBNIM NITIMA UZ KORIŠTENJE sleep_for FUNKCIJE
		// VODITI RAÈUNA O DIJELJENJU ZAJEDNIÈKIH RESURSA
	}
	catch (exception& err)
	{
		cout << err.what() << endl;
	}

	//KOD POREÐENJA DATUMA PRINTANJA NIJE POTREBNO POREDITI MINUTE, SAMO DATUM I SATE
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp3200.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	//cout << "Najcesce koristena zabranjena rijec je -> " << hp3200.GetTopZabranjenuRijec() << crt;

	Printer hp4000(hp3200);
	cout << "Prosjecan broj printanih stranica za " << danas << " je -> " << hp4000.GetProsjecanBrojStranicaPoDatumu(danas) << crt;
	//cout << "Najcesce koristena zabranjena rijec je -> " << hp4000.GetTopZabranjenuRijec() << crt;
	//hp4000.UkloniDokumente();//UKLANJA SVE DOKUMENTE KOJI NISU ZADOVOLJILI USLOVE ZA PRINTANJE
	cout << hp4000 << crt;

#pragma endregion
}
