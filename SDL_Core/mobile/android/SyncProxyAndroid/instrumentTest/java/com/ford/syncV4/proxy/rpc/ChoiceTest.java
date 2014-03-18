package com.ford.syncV4.proxy.rpc;

import com.ford.syncV4.proxy.RPCRequestFactory;
import com.ford.syncV4.proxy.constants.Names;
import com.ford.syncV4.proxy.rpc.enums.ImageType;

import junit.framework.TestCase;

import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.util.Vector;

/**
 * Created by Andrew Batutin on 10/3/13.
 */
public class ChoiceTest extends TestCase {

    public void testSecondaryTextShouldBeSet() throws Exception {
        Choice choice = new Choice();
        assertNotNull(choice);
        String testData = "text";
        choice.setSecondaryText(testData);
        JSONObject obj = getChoiceJSONObject(choice);
        String realData = (String) obj.get(Names.secondaryText);
        assertEquals("Deserialized message should match original one", testData, realData);
    }

    private JSONObject getChoiceJSONObject(Choice choice) throws JSONException {
        CreateInteractionChoiceSet choiceSet = createCreateInteractionChoiceSet(choice);
        JSONObject choiceSetJSON = choiceSet.serializeJSON((byte) 2);
        JSONArray choiceJSONList = (JSONArray) choiceSetJSON.getJSONArray("choiceSet");
        return (JSONObject) choiceJSONList.get(0);
    }

    public void testGetSecondaryTextShouldReturnSecondaryText() throws Exception {
        Choice choice = new Choice();
        String testData = "text";
        choice.setSecondaryText(testData);
        String realData = choice.getSecondaryText();
        assertEquals("Get ext should be == set text", testData, realData);
    }

    private CreateInteractionChoiceSet createCreateInteractionChoiceSet(Choice choice) {
        Vector<Choice> choiceVector = new Vector<Choice>();
        choiceVector.add(choice);
        CreateInteractionChoiceSet choiceSet = RPCRequestFactory.buildCreateInteractionChoiceSet();
        choiceSet.setChoiceSet(choiceVector);
        return choiceSet;
    }

    public void testSetTertiaryTextSetsTertiaryText() throws Exception {
        Choice choice = new Choice();
        assertNotNull(choice);
        String testData = "text";
        choice.setTertiaryText(testData);
        JSONObject obj = getChoiceJSONObject(choice);
        String realData = (String) obj.get(Names.tertiaryText);
        assertEquals("Deserialized message should match original one", testData, realData);
    }

    public void testGetTertiaryTextReturnsTertiaryText() throws Exception {
        Choice choice = new Choice();
        String testData = "text";
        choice.setTertiaryText(testData);
        String realData = choice.getTertiaryText();
        assertEquals("Get text should be == set text", testData, realData);
    }

    public void testSetSecondaryImageSetsSecondaryImage() throws Exception {
        Choice choice = new Choice();
        assertNotNull(choice);
        Image testImage = new Image();
        ImageType testData = ImageType.DYNAMIC;
        testImage.setImageType(ImageType.DYNAMIC);
        byte[] bulkData = new byte[100];
        testImage.setBulkData(bulkData);
        choice.setSecondaryImage(testImage);
        JSONObject obj = getChoiceJSONObject(choice);
        JSONObject realImage = (JSONObject) obj.get(Names.secondaryImage);
        ImageType realData = (ImageType) realImage.get(Names.imageType);
        assertEquals("Deserialized message should match original one", testData, realData);
    }

    public void testGetSecondaryImageReturnsSecondaryImage() throws Exception {
        Choice choice = new Choice();
        Image testImage = new Image();
        String testData = "Value";
        testImage.setValue(testData);
        choice.setSecondaryImage(testImage);
        Image realImage = choice.getSecondaryImage();
        String realData = realImage.getValue();
        assertEquals("Get ext should be == set text", testData, realData);
    }
}
