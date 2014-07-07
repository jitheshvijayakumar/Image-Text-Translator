
package com.gec.cse.miniproject.language;

import com.gec.cse.miniproject.CaptureActivity;
import com.gec.cse.miniproject.PreferencesActivity;

import android.app.Activity;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;


/**
 * Delegates translation requests to the appropriate translation service.
 */
public class Translator {

  public static final String BAD_TRANSLATION_MSG = "[Translation unavailable]";
  
  private Translator(Activity activity) {  
    // Private constructor to enforce noninstantiability
  }
  
  static String translate(Activity activity, String sourceLanguageCode, String targetLanguageCode, String sourceText) {   
    
    // Check preferences to determine which translation API to use--Google, or Bing.
    SharedPreferences prefs = PreferenceManager.getDefaultSharedPreferences(activity);
    String api = prefs.getString(PreferencesActivity.KEY_TRANSLATOR, CaptureActivity.DEFAULT_TRANSLATOR);
    
    // Delegate the translation based on the user's preference.
    if (api.equals(PreferencesActivity.TRANSLATOR_IMAGETEXTTRANSLATOR)) {
      
      // Get the correct code for the source language for this translation service.
      sourceLanguageCode = TranslatorImageText.toLanguage(
          LanguageCodeHelper.getTranslationLanguageName(activity.getBaseContext(), sourceLanguageCode));
      
      return MalayalamtransTranslatorImageText.translate(sourceLanguageCode, targetLanguageCode, sourceText);
    } 
    return BAD_TRANSLATION_MSG;
  }
}