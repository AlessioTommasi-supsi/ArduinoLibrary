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

