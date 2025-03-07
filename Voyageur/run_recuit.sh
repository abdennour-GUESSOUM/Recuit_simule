#!/bin/bash

# Fichier: recuit_simplifie.sh
# Description: Script qui exécute le recuit simulé avec différents paramètres 
# et sauvegarde les coûts optimaux dans un fichier

# Vérification de l'existence du programme
if [ ! -f "recuitVoyageur" ]; then
    echo "Compilation du programme..."
    make
    if [ $? -ne 0 ]; then
        echo "Erreur lors de la compilation. Arrêt du script."
        exit 1
    fi
fi

# Création du dossier pour les résultats
RESULTS_DIR="resultats_recuit"
mkdir -p "$RESULTS_DIR"

# Fichier pour sauvegarder les coûts optimaux
COUTS_FILE="couts_optimaux.txt"
echo "# Coûts optimaux pour différents paramètres du recuit simulé" > "$COUTS_FILE"
echo "# Date: $(date)" >> "$COUTS_FILE"
echo "" >> "$COUTS_FILE"

# Fonction pour exécuter le programme et extraire le coût optimal
run_recuit_and_get_cost() {
    local t_init=$1
    local t_fin=$2
    local alpha=$3
    local amplitude=$4
    local max_rep=$5
    local param_name=$6
    local param_value=$7
    
    echo "Exécution avec paramètres: Tinit=$t_init, Tfin=$t_fin, alpha=$alpha, amplitude=$amplitude, maxRep=$max_rep"
    
    # Préparation des entrées pour le programme
    cat <<EOF > input_params.txt
$t_init
$t_fin
$alpha
$amplitude
$max_rep
o
EOF
    
    # Exécution du programme avec redirection des sorties
    ./recuitVoyageur < input_params.txt > temp_output.txt 2>&1
    
    # Extraction du coût optimal
    cost=$(grep "Cout optimal" temp_output.txt | grep -o "[0-9]\+\.[0-9]\+" | head -1)
    
    # Sauvegarde du coût dans le fichier
    echo "$param_name=$param_value: $cost" >> "$COUTS_FILE"
    
    # Affichage du coût pour vérification
    echo "  -> Coût optimal: $cost"
    
    # Nettoyage des processus gnuplot qui pourraient rester en arrière-plan
    pkill gnuplot > /dev/null 2>&1
    
    # Pause courte pour s'assurer que tout est bien terminé
    sleep 1
}

# Test 1: Variation de la température initiale
echo "== Variation de la température initiale ==" | tee -a "$COUTS_FILE"
for t_init in 100 500 1000 5000; do
  echo "Test avec t_init = $t_init" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost $t_init 0.1 0.95 3 100 "Tinit" "${t_init}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Test 2: Variation du facteur temp fin
echo "== Variation du facteur Tf ==" | tee -a "$COUTS_FILE"
for t_fin in 10 1 0.1 0.01 0.001; do
  echo "Test avec Tfin = $t_fin" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost 1000 $t_fin 0.95 3 100 "Tfin" "${t_fin}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Test 3: Variation du facteur alpha
echo "== Variation du facteur alpha ==" | tee -a "$COUTS_FILE"
for alpha in 0.8 0.9 0.95 0.99; do
  echo "Test avec alpha = $alpha" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost 1000 0.1 $alpha 3 100 "Alpha" "${alpha}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Test 4: Variation de l'amplitude
echo "== Variation de l'amplitude ==" | tee -a "$COUTS_FILE"
for amplitude in 1 2 3 5 10; do
  echo "Test avec amplitude = $amplitude" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost 1000 0.1 0.95 $amplitude 100 "Amplitude" "${amplitude}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Test 5: Variation du nombre maximum de répétitions
echo "== Variation du nombre maximum de répétitions ==" | tee -a "$COUTS_FILE"
for max_rep in 100 200 500 1000 5000; do
  echo "Test avec max_rep = $max_rep" | tee -a "$COUTS_FILE"
  for i in {1..5}; do
    echo " Exécution $i/5" | tee -a "$COUTS_FILE"
    run_recuit_and_get_cost 1000 0.1 0.95 3 $max_rep "MaxRep" "${max_rep}_run${i}"
  done
  echo "" >> "$COUTS_FILE"
done
echo "" >> "$COUTS_FILE"

# Nettoyage des fichiers temporaires
rm -f input_params.txt temp_output.txt

echo "Tous les tests sont terminés."
echo "Les coûts optimaux ont été sauvegardés dans le fichier $COUTS_FILE"