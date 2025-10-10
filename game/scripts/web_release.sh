# Copy/replace all files from the local web release to the deployment on https://ckgames.ca/evades.
# Requires ssh access to ckgames@ckgames.ca.

scp -r release/web/* ckgames:/srv/site/evades/