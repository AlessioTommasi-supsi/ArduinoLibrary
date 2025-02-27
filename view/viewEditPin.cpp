#include "viewEditPin.h"

String viewEditPin::html = "";
Pin *viewEditPin::selectedPin = nullptr;

String viewEditPin::generateForm( int pinNumber)
{
    String form;

    form += R"(
        <div class="form-container">
        <form id="configurePinForm" action="action_submit_button" method="get">
            <p style="text-align: center;">Configure Pin<br>
                <label for="pinNumber">Number: )"+String(pinNumber)+R"(;
                </label>
                <br><br><br>
            </p>
            <label for="pinNumber">Pin Number:</label>
            <input type="text" id="pinNumber" name="pinNumber" value="23" required>
            <label for="pinType">Pin Type:</label>
            <select id="pinType" name="pinType" required> )";
    selectedPin = &SystemState::getInstance()->pinoutData->getPin(pinNumber);
    for (int i = 0; i < static_cast<int>(PinType::SIZE); ++i)
    {
        String pin_type = selectedPin->pinTypeToString(static_cast<PinType>(i));
        form += "<option value=\"" + pin_type + "\">" + pin_type + "</option>";
    }
    form += R"(
            </select>
            <label for="isInput">Is Input:</label>
            <select id="isInput" name="isInput" required onchange="showOutputValue() ">
                <option value="true">True</option>
                <option value="false">False</option>
            </select>
            <div id="outputValueContainer" style="display: none;">
                <label for="outputValue">Output Value:</label>
                <select id="outputValue" name="outputValue">
                    <option value="0">0V</option>
                    <option value="3.3">3.3V</option>
                </select>
            </div>
            <label for="pinNote">Note:</label>
            <input type="text" id="pinNote" name="pinNote">
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
        Devi passare il parametro PinNumber come se fosse un intero!
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
    // Creazione dell'header HTML con il foglio di stile CSS
    String html = viewGeneric::defaultCssHeader("Edit Pin");

    // Aggiunta del contenitore principale per form e valore del registro
    html+= viewEditPin::generateForm(pinNumber);
    html+= viewEditPin::addCustomScript(script);

    // Aggiunta del footer
    html += viewGeneric::defaultFooter();
    return html;
}

String viewEditPin::addCustomScript(String script)
{
    return script;
}

String viewEditPin::addDefaultScript(){
    String defaultScript = R"(
        <script>
            function showOutputValue() {
                var isInput = document.getElementById("isInput").value;
                var outputValueContainer = document.getElementById("outputValueContainer");
                if (isInput === "false") {
                    outputValueContainer.style.display = "block";
                } else {
                    outputValueContainer.style.display = "none";
                }
            }

            function applyConfig() {
                document.getElementById("configurePinForm").action = "form.html";
                document.getElementById("configurePinForm").submit();
            }

            function saveConfig() {
                document.getElementById("configurePinForm").action = "pinout_page";
                document.getElementById("configurePinForm").submit();
            }
        </script>

    )";
    return defaultScript;
}