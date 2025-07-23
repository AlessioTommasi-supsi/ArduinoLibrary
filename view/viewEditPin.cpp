#include "viewEditPin.h"

String viewEditPin::html = "";
Pin *viewEditPin::selectedPin = nullptr;

/********************************************************************************
 * il pin che viene passato deve essere un pin valido ovvero precedentemente    *
 *  aggiunto al pinout altrimenti si prendera un defaultpin ma le modifiche     *
 * non verraanno salvate poiche non saranno piu accessibili da pinout!          *
 * ******************************************************************************/
 

String viewEditPin::generateForm( int pinNumber)
{
    String form;
    selectedPin = &SystemState::getInstance()->pinoutData->getPin(pinNumber);
    form += R"(
        <div class="form-container">
        <form id="configurePinForm" action="action_submit_button" method="get">
            <p style="text-align: center;">Configure Pin<br>
                <label for="pinNumber">Number: )" +
            String(pinNumber) + R"(;
                </label>
                <br><br><br>
            </p>
            <label for="pin">Pin Number:</label>
            <input type="text" id="pin" name="pin" value=")" +String(pinNumber) + R"("required>
            <label for="pinType">Pin Type:</label>
            <select id="pinType" name="pinType" required> )";
    String current_type = selectedPin->getType();
    for (int i = 0; i < static_cast<int>(PinType::SIZE); ++i)
    {
        String pin_type = Pin::pinTypeToString(static_cast<PinType>(i));
        form += "<option value=\"" + pin_type + "\" " + (current_type == pin_type ? "selected" : "") + ">" + pin_type + "</option>";
    }
    form += R"(
            </select>
            <label for="isInput">Is Input:</label>
            <select id="isInput" name="isInput" required onchange="showOutputValue() ">
                <option value="true" >True</option>
                <option value="false" )";
    form += (!selectedPin->getIsInput()? "selected" : "");
    form += R"(
                >False</option>
            </select>
            <div id="outputValueContainer" style="display: )" ;
    form+= (selectedPin->getIsInput() ? "none" : "block");
    form+= R"(">
                <label for="outputValue">Output Value:</label>
                <select id="outputValue" name="outputValue">
                    <option value="0">0V</option>
                    <option value="3.3" )";
    form += (selectedPin->getVoltage() != 0  ? "selected" : "");
    form += R"(
                        >3.3V</option>
                </select>
            </div>
            <label for="pinNote">Note:</label>
            <input type="text" id="pinNote" name="pinNote" value=")";
    char noteBuffer[200];
    selectedPin->getNote(noteBuffer, sizeof(noteBuffer));
    form += String(noteBuffer);
    form += R"(">
            <button type="button" onclick="applyConfig() ">Apply</button>
            <label> </label> <!-- Add a space between the two buttons -->
            <button type="button" onclick="saveConfig() ">Save</button>
        </form>
    </div>
    )";

    return form;
}

String viewEditPin::generateHTML() // codice generato solo la prima volta quando nessun registro e stato ancora selezionato!
{
    html = viewGeneric::defaultCssHeader("Edit Pin");

    html += R"(passaggio parametri non corretto! <br> 
        Devi passare il parametro Pin come se fosse un intero!
    )";

    html += viewGeneric::defaultFooter();

    return html;
}

String viewEditPin::generateHTML(int pinNumber)
{
    String script = viewEditPin::addDefaultScript();
    return viewEditPin::generateHTML(pinNumber, script);
}

String viewEditPin::generateHTML(int pinNumber, String script)
{
    // Header ultra-minimalista con caricamento dinamico
    html = "<!DOCTYPE html><html><head>";
    html += "<meta charset='UTF-8'><meta name='viewport' content='width=device-width,initial-scale=1.0'>";
    html += "<title>Edit Pin</title>";
    html += "<style>body{margin:0;padding:60px 10px 120px;background:#f4f4f4}</style>";
    html += "</head><body>";
    
    // Tutto caricato dinamicamente
    html += viewGeneric::dynamicUpdateContentScript();
    html += viewGeneric::dynamicUpdateContent("navbar_area", "/navbarStyle", -1);
    html += viewGeneric::dynamicUpdateContent("editpin_content", "/editPinPageContent?pinNumber=" + String(pinNumber), -1);
    
    html += "<div id='navbar_area'></div>";
    html += viewGeneric::addNavbar();
    html += "<div id='editpin_content'><div style='text-align:center;padding:20px'>Loading pin editor...</div></div>";
    
    // Script aggiuntivo se fornito
    if (script.length() > 0) {
        html += "<script>" + script + "</script>";
    }
    
    html += "</body></html>";
    return html;
}

String viewEditPin::addCustomScript(String script)
{
    return script;
}

String viewEditPin::addDefaultScript(){
    // Script super compresso
    return "<script>function showOutputValue(){const i=document.getElementById('isInput').value,o=document.getElementById('outputValueContainer');o.style.display=i==='false'?'block':'none'}function applyConfig(){document.getElementById('configurePinForm').action='editPin';document.getElementById('configurePinForm').submit()}function saveConfig(){document.getElementById('configurePinForm').action='pinout';document.getElementById('configurePinForm').submit()}</script>";
}