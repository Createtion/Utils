# above install gh

1. conda install 
```python
bash Anaconda3-2022.05-Linux-x86_64.sh
```
> * yes to all
> * open .bashrc
```
# >>> conda initialize >>>
# !! Contents within this block are managed by 'conda init' !!
__conda_setup="$('/path/to/anaconda3/bin/conda' 'shell.bash' 'hook' 2> /dev/null)"
if [ $? -eq 0 ]; then
    eval "$__conda_setup"
else
    if [ -f "/path/to/anaconda3/etc/profile.d/conda.sh" ]; then
        . "/path/to/anaconda3/etc/profile.d/conda.sh"
    else
        export PATH="/path/to/anaconda3/bin:$PATH"
    fi
fi
unset __conda_setup
# <<< conda initialize <<<

```
2. gh install

```python
conda install gh   # input y 
gh auth login    # init auth
chose GitHub.com
HTTPS
yes
Paste an authentication token
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
gh repo clone Createtion/Projection
```



# push
git config --global  --unset https.https://github.com.proxy 
git config --global  --unset http.https://github.com.proxy 

