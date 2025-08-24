#include "viewGraph.h"
#include <set> 

String viewGraph::html = "";
String viewGraph::generateHTML()
{
    html = viewGeneric::defaultCssHeader("Graph View");

    html += "<h1>Graph View</h1>";

    html += "<h2>Registri:</h2>";
    std::vector<int> addresses = SystemState::getInstance()->getAllRegisterAddress();
    int i = 0;
    for (int addr : addresses)
    {
        addresses[i] += 1000; //operazione di shift
    }
    
    String apiFetch = "getRegisterValues";
    String apiFetchParam = "address";

    html += generateGraph(addresses, apiFetch, apiFetchParam);

    html += viewGeneric::defaultFooter();

    return html;
}


String viewGraph::generateGraph(std::vector<int> addresses, String apiFetch, String apiFetchParam)
{
    String var_html = "";

    // Menu a tendina per selezionare l'indirizzo del registro
    var_html += "<label for='           register-select'>Select:</label>";
    var_html += "<select id='register-select' onchange='updateGraph()'>";

    std::set<int> seen;

    for (int addr : addresses)
    {
        if (seen.find(addr) != seen.end()) {
            // Se l'indirizzo è già stato aggiunto, salto la generazione dell'option.
            continue;
        }
        seen.insert(addr);

        if (addr > 0 && addr < 1000) {
            var_html += "<option value='" + String(addr) + "'>" + String(addr) + "</option>";
        } else if (addr <= 0) 
        {
            var_html += "<option value='" + String(addr) + "'>Multiplex:" + String(-addr) + "</option>";
        }
        else if (addr >= 1000 && addr < 2000) {
            var_html += "<option value='" + String(addr - 1000) + "'>ModbusValue:" + String(addr - 1000) + "</option>";
        
        } else {
            var_html += "<option value='" + String(addr) + "'>Errore inserimento!" + String(addr - 3000) + "</option>";
        }
    }


    var_html += "</select>";

    // Contenitore per il grafico
    var_html += "<canvas id='myChart' width='400' height='200'></canvas>";

    // Script JavaScript per Chart.js e per gestire il grafico
    var_html += "<script src='https://cdn.jsdelivr.net/npm/chart.js' async></script>";
    var_html += "<script>";
    var_html += "let chart;";
    var_html += "function updateGraph() {";
    var_html += "  const address = document.getElementById('register-select').value;";
    var_html += "  fetch('/"+apiFetch+"?"+apiFetchParam+"=' + address)";
    var_html += "    .then(response => response.json())";
    var_html += "    .then(data => {";
    var_html += "      const labels = data.map((_, index) => index + 1);";
    var_html += "      const values = data;";
    var_html += "      const ctx = document.getElementById('myChart').getContext('2d');";
    var_html += "      if (chart) chart.destroy();";
    var_html += "      chart = new Chart(ctx, {";
    var_html += "        type: 'line',";
    var_html += "        data: {";
    var_html += "          labels: labels,";
    var_html += "          datasets: [{";
    var_html += "            label: 'Register Values',";
    var_html += "            data: values,";
    var_html += "            borderColor: 'rgba(75, 192, 192, 1)',";
    var_html += "            borderWidth: 2";
    var_html += "          }]";
    var_html += "        },";
    var_html += "        options: {";
    var_html += "          responsive: true,";
    var_html += "          animation: false,"; // Disabilita l'animazione
    var_html += "          scales: {";
    var_html += "            x: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Sample Index'";
    var_html += "              }";
    var_html += "            },";
    var_html += "            y: {";
    var_html += "              display: true,";
    var_html += "              title: {";
    var_html += "                display: true,";
    var_html += "                text: 'Value'";
    var_html += "              }";
    var_html += "            }";
    var_html += "          }";
    var_html += "        }";
    var_html += "      });";
    var_html += "    });";
    var_html += "}";
    var_html += "document.addEventListener('DOMContentLoaded', () => {";
    var_html += "  updateGraph();";                  // Per caricare il grafico all'avvio
    var_html += "  setInterval(updateGraph, 5000);"; // Aggiorna il grafico ogni 5 secondi
    var_html += "});";
    var_html += "</script>";

    return var_html;
}

String viewGraph::initCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    // Usa Chart.js moderno da CDN
    var_circle_progressbar_html += "<script src='https://cdn.jsdelivr.net/npm/chart.js@4.4.0/dist/chart.umd.js'></script>";
    
    // Container moderno con CSS di viewGeneric
    var_circle_progressbar_html += "<div class='charts-container'>";
    
    // Script di inizializzazione globale
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "var charts = {};"; // Oggetto per tracciare i grafici
    var_circle_progressbar_html += "var chartUpdateIntervals = {};"; // Traccia gli intervalli
    var_circle_progressbar_html += "</script>";
    
    return var_circle_progressbar_html;
}

String viewGraph::endCirularProgressBarGraph()
{
    String var_circle_progressbar_html = "";
    var_circle_progressbar_html += "</div>"; // Chiude charts-container
    
    // Script principale per gestione grafici
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += R"(
        // Funzione per creare/aggiornare grafico circolare
        function createOrUpdateChart(canvasId, percentage, used, total) {
            const canvas = document.getElementById(canvasId);
            if (!canvas) {
                console.error('Canvas non trovato:', canvasId);
                return;
            }
            
            const ctx = canvas.getContext('2d');
            
            // Distruggi grafico esistente se presente
            if (charts[canvasId]) {
                charts[canvasId].destroy();
            }
            
            // Determina colore basato su percentuale
            let color = '#2ecc71'; // Verde
            if (percentage > 80) color = '#e74c3c'; // Rosso
            else if (percentage > 60) color = '#f39c12'; // Arancione
            
            // Crea nuovo grafico SENZA ANIMAZIONI per migliori performance
            charts[canvasId] = new Chart(ctx, {
                type: 'doughnut',
                data: {
                    datasets: [{
                        data: [percentage, 100 - percentage],
                        backgroundColor: [color, '#ecf0f1'],
                        borderWidth: 0,
                        cutout: '75%'
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    animation: false, // ANIMAZIONI DISABILITATE per performance
                    plugins: {
                        tooltip: { enabled: false },
                        legend: { display: false }
                    }
                }
            });
            
            // Aggiorna etichetta centrale
            const labelElement = document.getElementById(canvasId.replace('canvas-', 'label-'));
            if (labelElement) {
                labelElement.textContent = percentage.toFixed(1) + '%';
            }
            
            // Aggiorna dettagli
            const detailsElement = document.getElementById(canvasId.replace('canvas-', 'details-'));
            if (detailsElement) {
                detailsElement.innerHTML = `
                    <div>Utilizzato: ${formatBytes(used)}</div>
                    <div>Totale: ${formatBytes(total)}</div>
                `;
            }
            
            // Aggiorna status
            const statusElement = document.getElementById(canvasId.replace('canvas-', 'status-'));
            if (statusElement) {
                let statusClass = 'status-good';
                let statusText = 'Normale';
                
                if (percentage > 80) {
                    statusClass = 'status-critical';
                    statusText = 'Critico';
                } else if (percentage > 60) {
                    statusClass = 'status-warning';
                    statusText = 'Attenzione';
                }
                
                statusElement.className = 'chart-status ' + statusClass;
                statusElement.textContent = statusText;
            }
        }
        
        // Funzione per aggiornare grafico da API
        async function updateChartFromAPI(apiUrl, canvasId) {
            try {
                const wrapper = document.getElementById(canvasId.replace('canvas-', 'wrapper-'));
                if (wrapper) wrapper.classList.add('chart-loading');
                
                const response = await fetch(apiUrl);
                if (!response.ok) throw new Error('Errore API: ' + response.status);
                
                const data = await response.json();
                const percentage = (data.used / data.total) * 100;
                
                createOrUpdateChart(canvasId, percentage, data.used, data.total);
                
                console.log('📊 Grafico aggiornato:', canvasId, percentage.toFixed(1) + '%');
            } catch (error) {
                console.error('❌ Errore aggiornamento grafico:', canvasId, error);
            } finally {
                const wrapper = document.getElementById(canvasId.replace('canvas-', 'wrapper-'));
                if (wrapper) wrapper.classList.remove('chart-loading');
            }
        }
        
        // Utility per formattare bytes
        function formatBytes(bytes) {
            if (bytes === 0) return '0 B';
            const k = 1024;
            const sizes = ['B', 'KB', 'MB', 'GB'];
            const i = Math.floor(Math.log(bytes) / Math.log(k));
            return parseFloat((bytes / Math.pow(k, i)).toFixed(2)) + ' ' + sizes[i];
        }
        
        // Cleanup function per fermare aggiornamenti
        function stopChartUpdates(canvasId) {
            if (chartUpdateIntervals[canvasId]) {
                clearInterval(chartUpdateIntervals[canvasId]);
                delete chartUpdateIntervals[canvasId];
            }
        }
    )";
    var_circle_progressbar_html += "</script>";
    
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value, String apiFetchData, int timeToUpdate)
{
    String var_circle_progressbar_html = "";
    float percentage = (circle_progressbar_used_value / circle_progressbar_total_value) * 100;
    
    // ID univoci per ogni elemento
    String baseId = "chart-" + circle_progressbar_label;
    String canvasId = "canvas-" + circle_progressbar_label;
    String wrapperId = "wrapper-" + circle_progressbar_label;
    String labelId = "label-" + circle_progressbar_label;
    String detailsId = "details-" + circle_progressbar_label;
    String statusId = "status-" + circle_progressbar_label;
    
    // Wrapper principale con nuovo CSS
    var_circle_progressbar_html += "<div id='" + wrapperId + "' class='chart-wrapper'>";
    
    // Titolo del grafico
    var_circle_progressbar_html += "<div class='chart-label'>" + circle_progressbar_label + "</div>";
    
    // Container canvas con etichetta centrale
    var_circle_progressbar_html += "<div class='chart-canvas-container'>";
    var_circle_progressbar_html += "<canvas id='" + canvasId + "' class='chart-canvas'></canvas>";
    var_circle_progressbar_html += "<div id='" + labelId + "' class='chart-center-label'>" + String(percentage, 1) + "%</div>";
    var_circle_progressbar_html += "</div>";
    
    // Dettagli utilizzo
    var_circle_progressbar_html += "<div id='" + detailsId + "' class='chart-details'>";
    var_circle_progressbar_html += "Utilizzato: " + String(circle_progressbar_used_value) + "<br>";
    var_circle_progressbar_html += "Totale: " + String(circle_progressbar_total_value);
    var_circle_progressbar_html += "</div>";
    
    // Status badge
    var_circle_progressbar_html += "<div id='" + statusId + "' class='chart-status status-good'>Normale</div>";
    
    var_circle_progressbar_html += "</div>"; // Chiude chart-wrapper
    
    // Script per inizializzazione e aggiornamento
    var_circle_progressbar_html += "<script>";
    var_circle_progressbar_html += "document.addEventListener('DOMContentLoaded', function() {";
    
    // Crea grafico iniziale
    var_circle_progressbar_html += "setTimeout(() => {";
    var_circle_progressbar_html += "createOrUpdateChart('" + canvasId + "', " + String(percentage) + ", " + String(circle_progressbar_used_value) + ", " + String(circle_progressbar_total_value) + ");";
    var_circle_progressbar_html += "}, 100);"; // Piccolo delay per assicurarsi che Chart.js sia caricato
    
    // Imposta aggiornamento automatico se richiesto
    if (timeToUpdate > 0) {
        var_circle_progressbar_html += "chartUpdateIntervals['" + canvasId + "'] = setInterval(() => {";
        var_circle_progressbar_html += "updateChartFromAPI('" + apiFetchData + "', '" + canvasId + "');";
        var_circle_progressbar_html += "}, " + String(timeToUpdate) + ");";
    }
    
    var_circle_progressbar_html += "});";
    var_circle_progressbar_html += "</script>";
    
    return var_circle_progressbar_html;
}

String viewGraph::generateCirularProgressBarGraph(String circle_progressbar_label, float circle_progressbar_used_value, float circle_progressbar_total_value)
{
    // Versione senza aggiornamento automatico
    return generateCirularProgressBarGraph(circle_progressbar_label, circle_progressbar_used_value, circle_progressbar_total_value, "", -1);
}

