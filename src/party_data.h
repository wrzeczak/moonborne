//------------------------------------------------------------------------------//
// party data -- contains enums and lists that map ideologies to pol_ts and the //
// like, written mostly for the purpose of keeping party.h clean.               //
//------------------------------------------------------------------------------//

#pragma once

#include "person.h" // for politics_t

//------------------------------------------------------------------------------

// general political classifications
enum {
	CENTRIST,
	CENTER_LEFT,		// generally neoliberal
	CENTER_RIGHT,		// generally neoconservatives
	LEFTIST,			// further than neoliberal but not socialist/marxist/etc.
	RIGHTIST,			// generally a traditionalist/conservative party, not fascist/national
	FAR_LEFT,			// catch-all for anything past leftist
	FAR_RIGHT,			// catch-all for anything past rightist
};

// more specific, these labels can overlap ie. "marxist left-racial party" or "national fascist party"
enum {
	NEOLIB,				// generally center-left parties fall here
	NEOCON,				// generally, center-right
	DEMSOC,				// generally, leftist
	LIBERTARIAN,		// generally, rightist
	CONSERVATIVE,		// rightist or far-right
	MILITARY,			// far-left or far-right, describes a militant/war-hawk party, not a military junta
	SOCIALIST,			// leftist or far-leftist
	MARXIST,			// far-left
	FASCIST,			// far-right
	SATIRICAL,			// centrist for ease of classification
	NATIONAL,			// rightist or far-right
	LEFT_ANARCHIST, 	// far-left
	RIGHT_ANARCHIST,	// far-right
	LEFT_RACIAL,		// far-left, a "racial" party is something like the black panthers or PLF
	RIGHT_RACIAL		// far right
};

//------------------------------------------------------------------------------

