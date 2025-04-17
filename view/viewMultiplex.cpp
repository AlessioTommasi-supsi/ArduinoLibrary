#include "viewMultiplex.h"


String viewMultiplex::Config(){
    String html = "";
    html = viewGeneric::basicHeader("Multiplex ConfigPage");
    html += viewGeneric::dynamicUpdateContentScript(); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("id_navbarStyle", "/navbarStyle", -1); //aggiungo script per aggiornamento dinamico
    html += viewGeneric::dynamicUpdateContent("id_fieldFormStyle", "/fieldFormStyle", -1);
    String page_content = "";
    page_content += viewGeneric::addNavbar();

    page_content += viewMultiplex::pageContent();
    
    page_content += viewGeneric::defaultFooter();

    return html+page_content;
}


String viewMultiplex::pageContent(){

    String content = R"rawliteral(
        <!-- Contenitore principale del form -->
        <div class="form-container">
            <h2>Configurazione Sistema</h2>
            <form id="configForm" action="multiplex_graph" method="get">
            
                <!-- Sezione: Seleziona il tipo di segnale -->
                <fieldset>
                    <legend>Seleziona il tipo di segnale</legend>
                    <label for="signalType">Tipo di segnale:</label>
                    <select id="signalType" name="signalType" onchange="updateSignalMessage()">
                        <option value="resistenza">Resistenza</option>
                        <option value="tensione_non_amplificato">Segnale di tensione non amplificato</option>
                        <option value="tensione_amp_331">Segnale di tensione con guadagno pari a 331</option>
                        <option value="tensione_amp_0.216">Segnale di tensione con guadagno pari a 0.216</option>
                    </select>
                </fieldset>
                
                <!-- Messaggio che compare in base alla selezione -->
                <div id="signalMessage" style="margin: 20px 0; font-weight: bold;"></div>
                
                <!-- Pulsante di invio -->
                <button type="submit">Invia Configurazione</button>
            </form>
        </div>
        
        <script>
            function updateSignalMessage() {
                var signalType = document.getElementById("signalType").value;
                var messageDiv = document.getElementById("signalMessage");
                        
                switch (signalType) {
                    case "resistenza":
                        messageDiv.textContent = "La resistenza PT100 la devi collegare tra il morsetto 8 e il morsetto 2 della morsettiera CN2.";
                        break;
                    case "tensione_non_amplificato":
                        messageDiv.textContent = "Devi collegare segnale al morsetto 7 della morsettiera CN2.";
                        break;
                    case "tensione_amp_331":
                        messageDiv.textContent = "Collegare al morsetto 6 della morsettiera CN2: max signal readed +5v, -5v.";
                        break;
                    case "tensione_amp_0.216":
                        messageDiv.textContent = "Collegare al morsetto 1 dekka morsettiera CN2.";
                        break;
                    default:
                        messageDiv.textContent = "";
                }
            }
            
            document.addEventListener("DOMContentLoaded", function() {
                updateSignalMessage();
            });
        </script>
        )rawliteral";
        
    return content;
}

/*Devo ancora modificare grafico, se prende in input un determinato numero di pin non mostra tutto menu a tendina ma fa diverso! solo signaltype!*/
String viewMultiplex::pinStartAndStopForm(int PinNumber, String signalType){
    String form = "";

    //form+= viewGeneric::dynamicUpdateContent("", "/pinStyle", -1); e richiesto da mettere al chiamante!
    form += R"(
        <div class="pin-container">
            <h3>
                <div class="pin-info">Tipo di segnale: )" + signalType + R"(</div>
            </h3>
            <div class="pin-actions">
                <form action="/multiplex_graph" method="get">
                    <label for="milliseconds">Milliseconds:</label>
                    <input type="text" id="milliseconds" name="milliseconds" required="" value="1000">
                    <input type="hidden" name="signalType" value=")" + signalType + R"(">
                    <input type="hidden" name="pinNumber" value=")" + String(PinNumber) + R"(">
                    <input type="hidden" name="action" value="start_recording">
                    <button type="submit" class="start">Start</button>
                </form>
                <form action="/multiplex_graph" method="get">
                    <input type="hidden" name="signalType" value=")" + signalType + R"(">
                    <input type="hidden" name="pinNumber" value=")" + String(PinNumber) + R"(">
                    <input type="hidden" name="action" value="stop_recording">
                    <button type="submit" class="stop">Stop</button>
                </form>
            </div>
        </div>
    )";


    
    return form;

}