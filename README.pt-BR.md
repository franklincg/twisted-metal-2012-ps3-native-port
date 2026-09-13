# Twisted Metal (2012) — Port nativo de PS3 para PC

**Projeto iniciado e coordenado por Franklin Wilster (FrankCG) — Brasil 🇧🇷**

[English](README.md) · [Como ajudar](docs/HOW_TO_HELP.md) · [Tarefas](docs/WORK_ITEMS.md) · [Progresso](docs/PROGRESS.md) · [Créditos](CREDITS.md)

> **Pesquisa em andamento, antes do menu. Não é uma versão jogável.** O alvo é o **Twisted Metal de 2012, do PlayStation 3**, não o jogo de 1995. O menu e o gameplay desta iniciativa ainda não foram comprovados.

## A ideia

Construir uma base nativa para Windows com colaboração aberta, aproveitando correções existentes e validando mudanças pequenas. Depois de uma versão fiel e estável, estudar SDK de mods, integração Blender/Cinema 4D, editor de mapas, localização e vozes PT-BR, melhorias gráficas, online comunitário e outras plataformas. Esses itens são planos, não recursos prontos nem promessas.

## O que está disponível nesta primeira entrega

Documentação, áreas de trabalho, tarefas detalhadas, um teste C sintético executável sem o jogo, fontes selecionados do modelo host histórico para revisão e ferramentas de auditoria/publicação. **O runtime completo e um build reproduzível do jogo ainda não estão neste pacote.** A exportação segura dos demais fontes é a tarefa PUB-001.

O workspace privado, o histórico interno do Git e os arquivos comerciais não são espelhados. Quem clona este material recebe apenas os arquivos selecionados para ele.

## Como as pessoas trabalham em paralelo

Cada pessoa escolhe uma tarefa, combina seu escopo em texto, trabalha em um fork/branch e envia um pull request. Testes, licença e revisão vêm antes da integração. Áudio, vídeo, memória, armazenamento e ferramentas têm áreas separadas, com dependências explícitas. Resolver um teste host não autoriza executar o jogo na máquina do mantenedor.

Há trabalho independente em representação de memória, coerência PPU/SPU, vida útil de contextos, documentação, build público, testes de áudio e entrada. Scheduler e integração do jogo seguem seus próprios requisitos. [O quadro de tarefas](docs/WORK_ITEMS.md) mostra o que pode começar e o que está bloqueado.

## Créditos

Franklin Wilster (FrankCG) é o iniciador e coordenador **desta iniciativa brasileira independente**. Isso não reivindica autoria do jogo original, autoria exclusiva de todo o código ou o primeiro port de Twisted Metal. Cada colaboração recebe crédito. ChatGPT e Codex são ferramentas de assistência, declaradas em [AI_USE.md](AI_USE.md), não garantia de que o código esteja correto.

## Segurança e limites

Não envie ISO, PKG, EBOOT, assets, firmware, chaves, saves ou dumps privados. Os testes públicos não precisam desses arquivos. Contribuições de desconhecidos não devem executar automaticamente no computador pessoal do mantenedor; a configuração de CI usa apenas ambientes descartáveis hospedados pelo GitHub.

Não há porcentagem geral confiável do port. O painel calcula apenas critérios definidos e identificados, separando resultados históricos, testes novos e integração ainda pendente.

Projeto não oficial e sem afiliação com os titulares do jogo. Publicar ferramentas em vez de assets reduz o conteúdo exposto, mas não elimina todos os riscos jurídicos. Licenças e créditos de terceiros são preservados.
