///////////////////////////////////////////////////////////////////////////////
// Name:        tests/masked/maskededit.cpp
// Purpose:     wxMaskedEdit unit test
// Author:      Julien Weinzorn
// Created:     2010-06-02
// RCS-ID:      $Id: ??????????????????????????????? $
// Copyright:   (c) 2009 Julien Weinzorn
///////////////////////////////////////////////////////////////////////////////

// ----------------------------------------------------------------------------
// headers
// ----------------------------------------------------------------------------

#include "testprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#include "wx/maskededit.h"

// ----------------------------------------------------------------------------
// test class
// ----------------------------------------------------------------------------

class MaskedEditTestCase : public CppUnit::TestCase
{
public:
    MaskedEditTestCase() { }

private:
    CPPUNIT_TEST_SUITE( MaskedEditTestCase );
        CPPUNIT_TEST( ApplyFormatCodesTest );
        CPPUNIT_TEST( GetPlainValueTest    );
        CPPUNIT_TEST( IsValidTest          );
        CPPUNIT_TEST( SetMaskTest          );
        CPPUNIT_TEST( SetDefaultValueTest  );
        CPPUNIT_TEST( AddChoiceTest        );
        CPPUNIT_TEST( AddChoicesTest       );
        CPPUNIT_TEST( SetMaskFieldTest     );
        CPPUNIT_TEST( AddChoiceFieldTest   );
        CPPUNIT_TEST( GetEmptyMaskTest     );
        CPPUNIT_TEST( GetFieldIndexTest    );
        CPPUNIT_TEST( GetMinFieldPositionTest );
        CPPUNIT_TEST( GetMaxFieldPositionTest );
    CPPUNIT_TEST_SUITE_END();

    void ApplyFormatCodesTest();
    void GetPlainValueTest();
    void IsValidTest();
    void SetMaskTest();
    void SetDefaultValueTest();
    void AddChoiceTest();
    void AddChoicesTest();
    void SetMaskFieldTest();
    void AddChoiceFieldTest();
    void GetEmptyMaskTest();
    void GetFieldIndexTest();
    void GetMinFieldPositionTest();
    void GetMaxFieldPositionTest();
    wxDECLARE_NO_COPY_CLASS(MaskedEditTestCase);
};

// ----------------------------------------------------------------------------
// CppUnit macros
// ----------------------------------------------------------------------------

CPPUNIT_TEST_SUITE_REGISTRATION( MaskedEditTestCase );
CPPUNIT_TEST_SUITE_NAMED_REGISTRATION( MaskedEditTestCase, "MaskedEditTestCase" );


// ----------------------------------------------------------------------------
// tests implementation
// ----------------------------------------------------------------------------

void MaskedEditTestCase::ApplyFormatCodesTest()
{
    wxArrayString formatCodes;
    wxMaskedEdit mask;
    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));

    static struct TestFormat
    {
        wxString mask;      
        wxString formatCodes;
        wxString test;
        wxString result;
    }
    maskedFormat[]=
    {
        {wxT("")                , wxT("F_") , wxT("azd")        , wxT("azd")},
        {wxT("###")             , wxT("F_") , wxT("1a")         , wxT("1a") },
        {wxT("###.###.###.###") , wxT("F_") , wxT("1.2.3.4")    , wxT("1.2.3.4")},
        {wxT("###.AAA.aC\\&")   , wxT("F!") , wxT("111.aaa.aZ&"), wxT("111.AAA.aZ&")},
        {wxT("#XX.")            , wxT("F!_"), wxT("3rt.")       , wxT("3rt.")},
        {wxT("#XX.")            , wxT("F!_"), wxT("3rt..")      , wxT("3rt..")},
        
        {wxT("###.|###.|###.|###"), wxEmptyString, wxT("1.2.3.4")    , wxT("1.2.3.4")},
        {wxT("###.A|AA|.aX|\\&")  , wxEmptyString, wxT("111.aaa.aZ&"), wxT("111.AAA.aZ&")},
        {wxT("#X|X.")             , wxEmptyString, wxT("3rt.")       , wxT("3rt.")},
    };

    for(unsigned int n = 0; n< WXSIZEOF( maskedFormat ) ; n++)
    {
        if(maskedFormat[n].formatCodes.Cmp(wxEmptyString) != 0)
        {
            mask = wxMaskedEdit(maskedFormat[n].mask,  maskedFormat[n].formatCodes);
        }
        else
        {
            mask = wxMaskedEdit(maskedFormat[n].mask, formatCodes);

        }
            CPPUNIT_ASSERT( maskedFormat[n].result.Cmp(mask.ApplyFormatCodes( maskedFormat[n].test)) == 0); 
    }
   
}

void MaskedEditTestCase::GetPlainValueTest()
{
    wxArrayString formatCodes;
    wxMaskedEdit mask;
    wxString tmp;

    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));
    formatCodes.Add(wxT("F!_"));
  
    static struct TestPlainValue
    {
        wxString mask;      
        wxString formatCodes;
        wxString test;
        wxString result;
    }
    maskedPlainValue[]=
    {
        {wxT("###.###.###.###") , wxT("F_")  , wxT("1.2.3.4")    , wxT("1.2.3.4")},
        {wxT("###.AAA.aC\\&")   , wxT("F!")  , wxT("111.aaa.aZ&"), wxT("111AAAaZ")},
        {wxT("#XX.")            , wxT("F!_") , wxT("3rt.")       , wxT("3rt")},
        
        {wxT("###.|###.|###.|###"), wxEmptyString, wxT("1.2.3.4")    , wxT("1.2.3.4")},
        {wxT("###.A|AA|.aX|\\&")  , wxEmptyString, wxT("111.aaa.aZ&"), wxT("111AAAaZ")},
        {wxT("#X|X.")             , wxEmptyString, wxT("3rt.")       , wxT("3rt")},
    };

    for(unsigned int n = 0; n< WXSIZEOF( maskedPlainValue ) ; n++)
    {

        if(maskedPlainValue[n].formatCodes.Cmp(wxEmptyString) != 0)
        {
            mask = wxMaskedEdit(maskedPlainValue[n].mask,  maskedPlainValue[n].formatCodes);
        }
        else
        {
            mask = wxMaskedEdit(maskedPlainValue[n].mask, formatCodes);
        }
        
        tmp = mask.ApplyFormatCodes(maskedPlainValue[n].test);

        CPPUNIT_ASSERT( maskedPlainValue[n].result.Cmp(mask.GetPlainValue(tmp)) == 0); 
    }
}


void MaskedEditTestCase::IsValidTest()
{ 
    wxMaskedEdit mask;
    wxString formatString; 
    static struct TestValid
    {
        wxString mask;
        wxString test;
        bool result;
    }
    listTest[]=
    {
        //same as maskedfield
        {wxT("")     , wxT("")     , true}, 
        {wxT("")     , wxT("azd")  , false},
        {wxT("###")  , wxT("123")  , true},
        {wxT("###")  , wxT("Az3")  , false},
        {wxT("###")  , wxT("wx.;") , false},
        {wxT("###")  , wxT("1A2")  , false},
        {wxT("###")  , wxT("1..")  , false},
        {wxT("###.") , wxT("1")    , true},
        {wxT("###.") , wxT("111.") , true},

        {wxT("AAa.#X*") , wxT("AZc.3,|") , true},
        {wxT("AAa.#X*") , wxT("AZc3,|")  , false}, 
        {wxT("AAa.#X*") , wxT("aZc.|.|") , false},
        {wxT("AAa.#X*") , wxT("")        , false}, 
        {wxT("AAa.#X*") , wxT("AZc.3,|4"), false},

        {wxT("AAa.\\*A") , wxT("AZa.*Aa"), false},
        
        {wxT("###\\*###") , wxT("") , false},
        {wxT("###\\*###") , wxT("123*593") , true},
        {wxT("###\\*###") , wxT("123456") , false},
        {wxT("###\\*###") , wxT("124\\*45") , false},

        //with more than one field
        {wxT("AAa|.#X*") , wxT("AZc.3,|") , true},
        {wxT("AA|a.#|X*"), wxT("AZc3,|")  , false},
        {wxT("AAa.#X*")  , wxT("aZc.|.|") , false},
        {wxT("A|Aa.#X*") , wxT("")        , false},
        {wxT("AAa.|#X*") , wxT("AZc.3,|4"), false},

        {wxT("#|##\\*#|##") , wxT("")        , false},
        {wxT("###\\*###")   , wxT("123*593") , true},
        {wxT("|###\\*#|##") , wxT("123456")  , false}, 
        {wxT("|###\\*###|") , wxT("124\\*45"), false},
        {wxT("|###\\*###|") , wxT("124*45")  , true},
        {wxT("Aa{5} A{5}")  , wxT("A")       , true},
    
        {wxT("###")    , wxT("1")  , true},
        {wxT("###")     , wxT("1")  , true},
        {wxT("###")    , wxT("111")  , true},
        {wxT("###")    , wxT("-111") , true},
 

    };

    for(unsigned int n = 0; n< WXSIZEOF(listTest); n++)
    {
        printf("N= %d\n", n);
        if(!listTest[n].mask.Contains('|'))
            mask = wxMaskedEdit(listTest[n].mask, wxT("F_-"));
        else
            mask = wxMaskedEdit(listTest[n].mask, wxArrayString());

        formatString = mask.ApplyFormatCodes(listTest[n].test); 
        CPPUNIT_ASSERT_EQUAL(listTest[n].result ,mask.IsValid(formatString)); 
    }

}


void MaskedEditTestCase::SetMaskTest()
{
    wxMaskedEdit mask;
    wxString formatCode;
    wxArrayString formatCodes;
    static struct TestMask
    {
        wxString mask;
        wxString newMask;
        bool result;
    }
    masked[]=
    {
        {wxT("AAa.#X*") , wxT("AZc.3,|") , false},
        {wxT("AA|a.#|X*") , wxT("AZc3,|")  , false},
        {wxT("AAa.#X*") , wxT("AZc..") , true},
        {wxT("A|Aa.#X*") , wxT("")        , false},
        {wxT("AAa.|#X*") , wxT("AZc.3,|4"), false},
    }; 

    for(unsigned int n = 0; n< WXSIZEOF(masked); n++)
    {
        printf("N = %d\n", n);
        if(masked[n].mask.Contains('|'))
            mask = wxMaskedEdit(masked[n].mask, formatCodes);
        else
            mask = wxMaskedEdit(masked[n].mask, formatCode);
        
        CPPUNIT_ASSERT( mask.SetMask(masked[n].newMask) == masked[n].result); 
    }
}


void MaskedEditTestCase::SetDefaultValueTest()
{
    static struct TestDefaultValue
    {
        wxString mask;      
        wxString formatCodes;
        wxString test;
        bool result;


    }
    maskedDefaultValue[]=
    {
        //same as wxMaskedField
        {wxT("###.###.###.###") , wxT("F_")  , wxT("111.111.111.1") , true},
        {wxT("###.AAA.aC\\&")   , wxT("F!")  , wxT("111.aaa.aZ&")   , true},
        {wxT("#XX.")            , wxT("F!_") , wxT("3rt.")          , true},
        {wxT("###.")            , wxT("F_")  , wxT("1.")            , false},
        {wxT("###")            , wxT("F_")  , wxT("111")            , true},

        {wxT("###|.###|.###|.###") ,wxEmptyString , wxT("111.111.111.111") , true},
        {wxT("###|.AAA.aC\\&")   ,  wxEmptyString   , wxT("111.AAA.aZ&")   , true},
        {wxT("#X|X.")            ,  wxEmptyString   , wxT("3rt.")          , true},
        {wxT("#|##.")            ,  wxEmptyString   , wxT("1.")            , false},

    };


    wxString tmp;
    wxString res;
    wxMaskedEdit mask;
    for(unsigned int n = 0; n< WXSIZEOF(maskedDefaultValue); n++)
    {
        if( maskedDefaultValue[n].formatCodes == wxEmptyString)
        {
            mask = wxMaskedEdit(maskedDefaultValue[n].mask, wxArrayString());

        }
        else
        {
            mask = wxMaskedEdit(maskedDefaultValue[n].mask, maskedDefaultValue[n].formatCodes);
        }

        tmp = mask.ApplyFormatCodes(maskedDefaultValue[n].test);
        CPPUNIT_ASSERT_EQUAL( maskedDefaultValue[n].result, mask.SetDefaultValue(tmp)  );    
    }


}


void MaskedEditTestCase::AddChoiceTest()
{
    wxString formatCode;
    wxArrayString formatCodes;

    static struct TestChoice
    {
        wxString mask;
        wxString choice;
        bool result;
    }
    masked[]=
    {
        {wxT("###.###") , wxT("111.111") , true},
        {wxT("###|.###") , wxT("111.111")  , false},
        {wxT("###.###") , wxT("1.1") , false},
        {wxT("###|.###") , wxT("111.111")  , false},
        {wxT("###.###") , wxT("1a2.34t") , false},
        {wxT("###|.###") , wxT("123|.111")  , false},
    }; 

    for(unsigned int n = 0; n< WXSIZEOF(masked); n++)
    {
        wxMaskedEdit mask;
        if(masked[n].mask.Contains('|'))
            mask = wxMaskedEdit(masked[n].mask, formatCodes);
        else
            mask = wxMaskedEdit(masked[n].mask, formatCode);

        CPPUNIT_ASSERT( mask.AddChoice(masked[n].choice) == masked[n].result); 
    }


}
//FIXME how to test this
void MaskedEditTestCase::AddChoicesTest()
{
    
}

void MaskedEditTestCase::SetMaskFieldTest()
{
    wxMaskedEdit mask;
    
    static struct TestMask
    {
        wxString mask;
        wxString newMask;
        unsigned int field;
        bool result;
    }
    masked[]=
    {
        {wxT("AAa.#X*")   , wxT("AZc.3,")  , 0 , true},
        {wxT("AA|a.#|X*") , wxT("AZc#,|")  , 1 , true},
        {wxT("AAa.#X*")   , wxT("AZc#.")   , 1 , false},
        {wxT("A|Aa.#X*")  , wxT("")        , 0 , true},
        {wxT("AAa.|#X*")  , wxT("AZc.3,|4"), 2 , false},
    }; 

    for(unsigned int n = 0; n< WXSIZEOF(masked); n++)
    {
        mask = wxMaskedEdit(masked[n].mask, wxT(""));
        CPPUNIT_ASSERT_EQUAL(masked[n].result , mask.SetMask(masked[n].field, masked[n].newMask)); 
    }
   
}

void MaskedEditTestCase::AddChoiceFieldTest()
{
    wxString formatCode;
    wxArrayString formatCodes;

    static struct TestChoice
    {
        wxString mask;
        wxString choice;
        unsigned int field;
        bool result;
    }
    masked[]=
    {
        {wxT("###.###")  , wxT("111.111") , 0 , true},
        {wxT("###|.###") , wxT("111.111") , 0 , false},
        {wxT("###.###")  , wxT("1.1")     , 0 , false},
        {wxT("###|.###") , wxT("111")     , 0 , true},
        {wxT("###|.###") , wxT("111")     , 1 , false},
        {wxT("###.###")  , wxT("1a2.34t") , 1 , false},
        {wxT("###|.###") , wxT("123|.111"), 1 , false},
    }; 

    for(unsigned int n = 0; n< WXSIZEOF(masked); n++)
    {
        wxMaskedEdit mask;
        if(masked[n].mask.Contains('|'))
            mask = wxMaskedEdit(masked[n].mask, formatCodes);
        else
            mask = wxMaskedEdit(masked[n].mask, formatCode);

        CPPUNIT_ASSERT( mask.AddChoice(masked[n].field, masked[n].choice) == masked[n].result); 
    }
}

void MaskedEditTestCase::GetEmptyMaskTest()
{
    static struct TestEmptyMask
    {
        wxString mask;
        wxString result;


    }
    maskedEmpty[]=
    {
        //Same as Masked field
        {wxT("###.###.###.###") , wxT("   .   .   .   ")},
        {wxT("###.AAA.aC\\&")   , wxT("   .   .  &")},
        {wxT("#XX.")            , wxT("   .")},
        {wxT("(###) - ###-####"), wxT("(   ) -    -    ")},
 
        //With multiple fields
        {wxT("###|.###.|###.|###") , wxT("   .   .   .   ")},
        {wxT("##|#|.AAA.|aC|\\&")   , wxT("   .   .  &")},
        {wxT("#X|X.")            , wxT("   .")},
        {wxT("(###)| - |###|-|####"), wxT("(   ) -    -    ")}
   };


    for(unsigned int n = 0; n< WXSIZEOF(maskedEmpty); n++)
    {
        wxMaskedEdit mask(maskedEmpty[n].mask, wxT("F"));
        CPPUNIT_ASSERT( mask.GetEmptyMask().Cmp(maskedEmpty[n].result) == 0 );
    }


}
void MaskedEditTestCase::GetFieldIndexTest()
{    
    static struct TestFieldIndex
    {
        wxString mask;
        unsigned int test;
        unsigned int result;
    }
    maskedTest[]=
    {
        //Same as Masked field
        {wxT("###.###.###.###") , 0  , 0},
        {wxT("###.###.###.###") , 1  , 0},
        {wxT("###.###.###.###") , 14 , 0},
        {wxT("###.###.###.###") , 15 , 0},

        //With multiple fields
        {wxT("###|.###.|###.|###") , 0 , 0},
        {wxT("###|.###.|###.|###") , 2 , 0},
        {wxT("###|.###.|###.|###") , 3 , 1},
        {wxT("###|.###.|###.|###") , 4 , 1},
        {wxT("###|.###.|###.|###") , 7 , 1},
        {wxT("###|.###.|###.|###") , 8 , 2},
        {wxT("###|.###.|###.|###") , 10 ,2},
        {wxT("###|.###.|###.|###") , 21, 0}
   };


    for(unsigned int n = 0; n< WXSIZEOF(maskedTest); n++)
    {
        wxMaskedEdit mask(maskedTest[n].mask, wxT("F"));
        CPPUNIT_ASSERT_EQUAL(maskedTest[n].result,  mask.GetFieldIndex(maskedTest[n].test));
    }



}
void MaskedEditTestCase::GetMinFieldPositionTest()
{   
    static struct TestMinFieldIndex
    {
        wxString mask;
        unsigned int test;
        unsigned int result;
    }
    maskedTest[]=
    {
        //Same as Masked field
        {wxT("###.###.###.###") , 0   , 0},
        {wxT("###.###.###.###") , 1   , 0},
        {wxT("###.###.###.###") , 14  , 0},
        {wxT("###.###.###.###") , 15  , 0},

        //With multiple fields
        {wxT("###|.###.|###.|###") , 0 , 0},
        {wxT("###|.###.|###.|###") , 1 , 3},
        {wxT("###|.###.|###.|###") , 2 , 8},
        {wxT("###|.###.|###.|###") , 3 , 12},
        {wxT("###|.###.|###.|###") , 4 , 0},
   };


    for(unsigned int n = 0; n< WXSIZEOF(maskedTest); n++)
    {
        wxMaskedEdit mask(maskedTest[n].mask, wxT("F"));
        CPPUNIT_ASSERT_EQUAL(maskedTest[n].result,  mask.GetMinFieldPosition(maskedTest[n].test));
    }

}

void MaskedEditTestCase::GetMaxFieldPositionTest()
{  
    static struct TestMasFieldIndex
    {
        wxString mask;
        unsigned int test;
        unsigned int result;
    }
    maskedTest[]=
    {
        //Same as Masked field
        {wxT("###.###.###.###") , 0  , 14},
        {wxT("###.###.###.###") , 1  , 0},

        //With multiple fields
        {wxT("###|.###.|###.|###") , 0 , 2},
        {wxT("###|.###.|###.|###") , 1 , 7},
        {wxT("###|.###.|###.|###") , 2 , 11},
        {wxT("###|.###.|###.|###") , 3 , 14},
        {wxT("###|.###.|###.|###") , 4 , 0},
   };


    for(unsigned int n = 0; n< WXSIZEOF(maskedTest); n++)
    {
        wxMaskedEdit mask(maskedTest[n].mask, wxT("F"));
        CPPUNIT_ASSERT_EQUAL(maskedTest[n].result,  mask.GetMaxFieldPosition(maskedTest[n].test));
    }
}

