
package com.gec.cse.miniproject.language;

import java.net.URLEncoder;

import org.apache.http.client.HttpClient;
import org.apache.http.client.ResponseHandler;
import org.apache.http.client.methods.HttpGet;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.impl.client.DefaultHttpClient;

import android.util.Log;

import com.gec.cse.miniproject.CaptureActivity;
import com.google.api.GoogleAPI;
import com.google.api.translate.Language;


public class TranslatorMalayalam {
  private static final String TAG = TranslatorMalayalam.class.getSimpleName();
 

  private TranslatorMalayalam() {  
    // Private constructor to enforce noninstantiability
  }

  static String translate(String sourceLanguageCode, String targetLanguageCode, String inputText) {   
    Log.d(TAG, sourceLanguageCode + " -> " + targetLanguageCode);
    
    String sourceText = inputText.replaceAll("[^A-Za-z0-9 ]", " ").toLowerCase();
    
    // Truncate excessively long strings. Limit for Google Translate is 5000 characters
    if (sourceText.length() > 4500) {
      sourceText = sourceText.substring(0, 4500);
    }
     try {
        String source = URLEncoder.encode(sourceText, "UTF-8");
        Log.v(TAG, "Http request test "+source);
        HttpClient Client = new DefaultHttpClient();
        String URL = "http://127.0.0.1:8080/android/?txtSrc="+source;
        String SetServerString = "";
        
        // Create Request to server and get response

         HttpGet httpget = new HttpGet(URL);
         ResponseHandler<String> responseHandler = new BasicResponseHandler();
         SetServerString = Client.execute(httpget, responseHandler);

          // Show response on activity 
    	return SetServerString;
      //return Translate.DEFAULT.execute(sourceText, Language.fromString(sourceLanguageCode),Language.fromString(targetLanguageCode));
    } catch (Exception e) {
      Log.e(TAG, "Caught exeption in translation request.");
      return Translator.BAD_TRANSLATION_MSG;
    }
  }

  public static String toLanguage(String languageName) throws IllegalArgumentException {   
    // Convert string to all caps
    String standardizedName = languageName.toUpperCase();
    
    // Replace spaces with underscores
    standardizedName = standardizedName.replace(' ', '_');
    
    // Remove parentheses
    standardizedName = standardizedName.replace("(", "");   
    standardizedName = standardizedName.replace(")", "");
    
    try {
      return Language.valueOf(standardizedName).toString();
    } catch (IllegalArgumentException e) {
      Log.e(TAG, "Not found--returning default language code");
      return CaptureActivity.DEFAULT_TARGET_LANGUAGE_CODE;
    }
  }
}
