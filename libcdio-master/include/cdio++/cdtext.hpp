/*
    Copyright (C) 2005, 2008, 2012 Rocky Bernstein <rocky@gnu.org>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/** \file cdtext.hpp
 *  \brief methods relating to CD-Text information. This file
 *  should not be #included directly.
 */

/*! Return string representation of the enum values above */
const char *field2str (cdtext_field_t i) 
{
  return cdtext_field2str (i);
}

const char *genre2str (cdtext_genre_t i) 
{
  return cdtext_genre2str (i);
}

const char *lang2str (cdtext_lang_t i) 
{
  return cdtext_lang2str (i);
}

/*! returns an allocated string associated with the given field.  NULL is
  returned if key is CDTEXT_INVALID or the field is not set.
  
  The user needs to free the string using cdio_free() when done with it.

  @see getConst to retrieve a constant string that doesn't
  have to be freed.
  
*/
char *get (cdtext_field_t key, track_t i_track) 
{
  return cdtext_get (p_cdtext, key, i_track);
}

/*! returns a const string associated with the given field.  NULL is
  returned if key is CDTEXT_INVALID or the field is not set.
  
  Don't use the string when the cdtext object (i.e. the CdIo_t object
  you got it from) is no longer valid.

  @see cdio_get to retrieve an allocated string that persists past the
  cdtext object.

*/
const char *getConst (cdtext_field_t key, track_t i_track) 
{
  return cdtext_get_const (p_cdtext, key, i_track);
}

/*! 
  sets cdtext's keyword entry to field 
*/
void set (cdtext_field_t key, track_t i_track, const uint8_t *value, const char *charset) 
{
  cdtext_set (p_cdtext, key, value, i_track, charset);
}

/*!
  returns the selected language
*/
cdtext_lang_t getLanguage()
{
  return cdtext_get_language(p_cdtext);
}

/*!
  selects a language
*/
bool selectLanguage(cdtext_lang_t lang)
{
  return cdtext_select_language(p_cdtext, lang);
}

/*!
 returns a list of available languages
*/
cdtext_lang_t *listLanguages()
{
  return cdtext_list_languages(p_cdtext);
}


/* 
 * Local variables:
 *  c-file-style: "gnu"
 *  tab-width: 8
 *  indent-tabs-mode: nil
 * End:
 */
