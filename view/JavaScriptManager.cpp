#include "JavaScriptManager.h"

String JavaScriptManager::getFetchContentScript() {
    String script = R"(
        <script>
             // Funzione per caricare dinamicamente i dati dal backend
            async function fetchContent(api) {
                try {
                    // Mostra il messaggio di caricamento (se esiste)
                    const loadingDiv = document.getElementById('loading');
                    if (loadingDiv) loadingDiv.style.display = 'block';

                    // Effettua la richiesta GET
                    const response = await fetch(api);
                    if (!response.ok) {
                        throw new Error('Errore durante il caricamento dei dati: ' + response.status);
                    }
                    return await response.text(); // Restituisce il contenuto della risposta
                } catch (error) {
                    console.error(error);
                    const loadingDiv = document.getElementById('loading');
                    if (loadingDiv) {
                        loadingDiv.innerText = 'Errore durante il caricamento dei dati';
                    }
                    return null; // In caso di errore, restituisce null
                }
            }
        </script>
    )";
    return script;
}

String JavaScriptManager::getDynamicUpdateContentScript() {
    String script = "";
    
    script += getFetchContentScript();

    script += R"(
        <script>
            // Variabile globale per tenere traccia dell'indice dei contenuti aggiunti
            let contentIndex = 0;
            // Funzione per gestire l'aggiunta o la sostituzione del contenuto
            function updateContent(divId, content, replace) {
                if (replace) {
                    // Sostituisci il contenuto del div esistente
                    const existingDiv = document.getElementById(divId);
                    if (existingDiv) {
                        existingDiv.innerHTML = content;
                    } else {
                        console.error('Div con ID ' + divId + ' non trovato per la sostituzione.');
                    }
                } else {
                    // Aggiungi un nuovo div con contenuto sotto
                    const newContentDiv = document.createElement('div');
                    newContentDiv.id = `contentAdd_${contentIndex}`;
                    newContentDiv.className = 'content';
                    newContentDiv.innerHTML = content;
                    contentIndex++;

                    const loadButton = document.getElementById('loadButton');
                    if (loadButton) {
                        document.body.insertBefore(newContentDiv, loadButton);
                    } else {
                        document.body.appendChild(newContentDiv);
                    }
                }
            }

            // Funzione per inizializzare il caricamento
            async function loadPageContent(api, divId, timeToUpdate) {
                const content = await fetchContent(api);
                if (content) {
                    updateContent(divId, content, timeToUpdate > 0);
                }

                // Se il refresh è attivo, imposta un intervallo
                if (timeToUpdate > 0) {
                    setInterval(async () => {
                        const refreshedContent = await fetchContent(api);
                        if (refreshedContent) {
                            updateContent(divId, refreshedContent, true); // Sostituisce il contenuto
                        }
                    }, timeToUpdate);
                }
            }

            // Funzione globale per gestire i click sui bottoni dei servizi (DEVE essere definita prima del caricamento dei bottoni)
            async function fetchData(button, url) {
                console.log('🔗 Chiamando URL:', url); // LOG dell'URL
                
                // Aggiungi classe loading al bottone
                button.classList.add('loading-state');
                
                try {
                    // Determina se l'URL è completo (http/https) o relativo
                    let fetchUrl;
                    let isCrossOrigin = false;
                    
                    if (url.startsWith('http://') || url.startsWith('https://')) {
                        // URL completo - chiamata diretta
                        fetchUrl = url;
                        isCrossOrigin = !url.includes(window.location.hostname);
                        console.log('🌐 URL esterno:', fetchUrl, isCrossOrigin ? '(Cross-Origin)' : '(Same-Origin)');
                    } else {
                        // URL relativo - chiamata locale all'ESP
                        fetchUrl = url;
                        console.log('🏠 URL locale ESP:', fetchUrl);
                    }
                    
                    const response = await fetch(fetchUrl, {
                        mode: isCrossOrigin ? 'no-cors' : 'cors', // Usa no-cors per evitare errori CORS
                        method: 'GET'
                    });
                    
                    if (isCrossOrigin) {
                        // Per richieste cross-origin con no-cors, non possiamo leggere la risposta
                        // ma possiamo assumere che sia andata a buon fine se non ci sono errori
                        console.log('✅ Comando cross-origin inviato (no-cors mode):', fetchUrl);
                        setTimeout(() => {
                            button.classList.remove('loading-state');
                        }, 2000);
                    } else if (response.ok) {
                        console.log('✅ Comando eseguito con successo per:', fetchUrl);
                        setTimeout(() => {
                            button.classList.remove('loading-state');
                        }, 2000);
                    } else {
                        throw new Error('Risposta del server non valida: ' + response.status);
                    }
                } catch (error) {
                    console.error('❌ Errore durante la richiesta:', error);
                    
                    // Verifica se è un errore CORS specifico
                    if (error.name === 'TypeError' && 
                        (error.message.includes('CORS') || error.message.includes('NetworkError'))) {
                        console.log('🔄 Tentativo alternativo per CORS...');
                        
                        // Prova con una richiesta diretta tramite iframe nascosto o popup
                        try {
                            // Usa window.open per bypassare CORS (apre e chiude velocemente)
                            const popup = window.open(url, '_blank', 'width=1,height=1,top=0,left=0');
                            if (popup) {
                                setTimeout(() => {
                                    popup.close();
                                }, 1000);
                                console.log('✅ Comando inviato tramite popup:', url);
                            } else {
                                // Fallback: apri in nuova tab (l'utente dovrà chiuderla)
                                window.open(url, '_blank');
                                console.log('✅ Comando aperto in nuova tab:', url);
                            }
                        } catch (popupError) {
                            console.log('ℹ️ CORS bypassato - comando probabilmente eseguito:', url);
                        }
                    } else {
                        // Solo per errori non-CORS mostra l'alert
                        alert('Errore durante l\'esecuzione del comando: ' + error.message);
                    }
                    
                    // Rimuovi classe loading dopo un breve delay
                    setTimeout(() => {
                        button.classList.remove('loading-state');
                    }, 1500);
                }
            }
        </script>
    )";

    return script;
}

String JavaScriptManager::getDynamicUpdateContent(String divId, String api, int timeToUpdate) {
    String var_html = "";
    var_html += R"(
    <script> 
        // Esegue il caricamento iniziale
        try {
            loadPageContent(')" + api + R"(', ')" + divId + R"(', )" + String(timeToUpdate) + R"();
        } catch (error) {
            console.error('Errore durante il caricamento:', error);
        }
    </script>
    )";
    return var_html;
}